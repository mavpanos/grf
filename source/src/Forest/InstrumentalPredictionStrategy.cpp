#include <vector>
#include "InstrumentalPredictionStrategy.h"

InstrumentalPredictionStrategy::InstrumentalPredictionStrategy(size_t instrument_varID,
                                                               size_t treatment_varID,
                                                               size_t dependent_varID,
                                                               std::unordered_map<size_t, std::vector<double>>* responses):
   instrument_varID(instrument_varID),
   treatment_varID(treatment_varID),
   dependent_varID(dependent_varID),
   responses(responses) {}

std::vector<double> InstrumentalPredictionStrategy::predict(std::unordered_map<size_t, double>& weights_by_sampleID) {
  // Compute the relevant averages.
  double average_instrument = 0.0;
  double average_treatment = 0.0;
  double average_response = 0.0;

  for (auto it = weights_by_sampleID.begin(); it != weights_by_sampleID.end(); ++it) {
    size_t neighborID = it->first;
    double weight = it->second;

    average_instrument += weight * responses->at(instrument_varID)[neighborID];
    average_treatment += weight * responses->at(treatment_varID)[neighborID];
    average_response += weight * responses->at(dependent_varID)[neighborID];
  }

  // Finally, calculate the prediction.
  double numerator = 0.0;
  double denominator = 0.0;
  for (auto it = weights_by_sampleID.begin(); it != weights_by_sampleID.end(); ++it) {
    size_t neighborID = it->first;
    double weight = it->second;

    double instrument = responses->at(instrument_varID)[neighborID];
    double treatment = responses->at(treatment_varID)[neighborID];
    double response = responses->at(dependent_varID)[neighborID];

    numerator += weight * (instrument - average_instrument) * (response - average_response);
    denominator += weight * (instrument - average_instrument) * (treatment - average_treatment);
  }

  return { numerator / denominator };
}