// kate: replace-tabs off; indent-width 4; indent-mode normal
// vim: ts=4:sw=4:noexpandtab
/*

Copyright (c) 2010--2011,
François Pomerleau and Stephane Magnenat, ASL, ETHZ, Switzerland
You can contact the authors at <f dot pomerleau at gmail dot com> and
<stephane at magnenat dot net>

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ETH-ASL BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "Core.h"

// NullMatcher
template<typename T>
void MetricSpaceAligner<T>::NullMatcher::init(
	const DataPoints& filteredReference,
	bool& iterate)
{
	
}

template<typename T>
typename MetricSpaceAligner<T>::Matches MetricSpaceAligner<T>::NullMatcher::findClosests(
	const DataPoints& filteredReading,
	const DataPoints& filteredReference,
	bool& iterate)
{
	return Matches();
}

template struct MetricSpaceAligner<float>::NullMatcher;
template struct MetricSpaceAligner<double>::NullMatcher;



// KDTreeMatcher
template<typename T>
MetricSpaceAligner<T>::KDTreeMatcher::KDTreeMatcher(const int knn, const double epsilon, const NNSearchType searchType):
	knn(knn),
	epsilon(epsilon),
	searchType(searchType),
	featureNNS(0)
{
}

template<typename T>
MetricSpaceAligner<T>::KDTreeMatcher::~KDTreeMatcher()
{
	assert(featureNNS);
	delete featureNNS;
}

template<typename T>
void MetricSpaceAligner<T>::KDTreeMatcher::init(
	const DataPoints& filteredReference,
	bool& iterate)
{
	// build and populate NNS
	if (featureNNS)
		delete featureNNS;
	featureNNS = NNS::create(filteredReference.features, filteredReference.features.rows() - 1, searchType, NNS::TOUCH_STATISTICS);
}

template<typename T>
typename MetricSpaceAligner<T>::Matches MetricSpaceAligner<T>::KDTreeMatcher::findClosests(
	const DataPoints& filteredReading,
	const DataPoints& filteredReference,
	bool& iterate)
{
	
	const int pointsCount(filteredReading.features.cols());
	Matches matches(
		typename Matches::Dists(knn, pointsCount),
		typename Matches::Ids(knn, pointsCount)
	);
	
	this->visitCounter += featureNNS->knn(filteredReading.features, matches.ids, matches.dists, knn, epsilon, NNS::ALLOW_SELF_MATCH);
	
	return matches;
}

template struct MetricSpaceAligner<float>::KDTreeMatcher;
template struct MetricSpaceAligner<double>::KDTreeMatcher;