#include "GRT/GRT.h"
#include "istream.h"

using namespace GRT;

float analogInputToVoltage(float val) {
    return val / 1024.0 * 5.0;
}

float voltageToAcceleration(float val) {
    return (val - 1.65) / 1.65 * 3.0;
}

float analogInputToAcceleration(float val) {
    return voltageToAcceleration(analogInputToVoltage(val));
}

// Normalize by dividing each dimension by the total magnitude.
// Also add the magnitude as an additional feature.
vector<double> normalize(vector<double> input) {
    double magnitude;
    
    for (int i = 0; i < input.size(); i++) magnitude += (input[i] * input[i]);
    magnitude = sqrt(magnitude);
    for (int i = 0; i < input.size(); i++) input[i] /= magnitude;
    
    input.push_back(magnitude);
    
    return input;
}

ASCIISerialStream stream(9600, 3);
GestureRecognitionPipeline pipeline;

void setup() {
    stream.useUSBPort(0);
    stream.useNormalizer(normalize);
    useStream(stream);
    
    pipeline.addPreProcessingModule(MovingAverageFilter(5, 4));
    //pipeline.addFeatureExtractionModule(TimeDomainFeatures(10, 1, 3, false, true, true, false, false));
    pipeline.setClassifier(ANBC());
    usePipeline(pipeline);
}
