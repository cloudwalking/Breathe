// This is a simple breathing strip of LED.
// Customize using the Run and LED parameters below.

#include <SPI.h>
#include "LPD8806.h"

// Run parameters:
#define WAIT_FOR_KEYBOARD 0 // Use keyboard to pause/resume program.

// LED parameters:
#define LED_COUNT 16
#define DATA_PIN 6
#define CLOCK_PIN 12

LPD8806 strip = LPD8806(LED_COUNT, DATA_PIN, CLOCK_PIN);

void setup() {
  if (WAIT_FOR_KEYBOARD) {
    Serial.begin(9600);
    while (!Serial) { }		// Wait for serial to initalize.

  	Serial.println("Strike any key to start...");
  	while (!Serial.available()) { }
    Serial.read();
  }
  
  colorSetup();
}

void loop() {
  if (WAIT_FOR_KEYBOARD) {
    pauseOnKeystroke();
  }

  breath();
}

void pauseOnKeystroke() {
  if (Serial.available()) {
    Serial.read();
    Serial.println("Paused. Strike any key to resume...");
    showColorOff();
    while (!Serial.available());
    Serial.read();
  }
}

void colorSetup() {
  // Turn the strip on.
  strip.begin();
  
  // Refresh the strip.
  strip.show();
}

void showColorOff() {
  colorOff();
  strip.show();
}

void colorOff() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }
}

const int  CYCLE_MILLISECONDS = 5000; // 5 second breathing cycle.
const uint8_t KEYFRAMES[]  = {
   20, 21, 22, 24, 26, 28, 31, 34, 38, 41, 45, 50, 55, 60, 66, 73, 80, 87, 95,
   103, 112, 121, 131, 141, 151, 161, 172, 182, 192, 202, 211,
   220, 228, 236, 242, 247, 251, 254, 255

   , 254, 251, 247, 242, 236, 228,
   220, 211, 202, 192, 182, 172, 161, 151, 141, 131, 121, 112, 103, 95, 87,
   80, 73, 66, 60, 55, 50, 45, 41, 38, 34, 31, 28, 26, 24, 22, 21, 20, 20, 20,
   20, 20, 20, 20, 20, 20, 20, 20, 
};

unsigned long lastBreath = 0.0;
int keyframePointer = 0;

void breath() {
  int numKeyframes = sizeof(KEYFRAMES) - 1;
  float period = CYCLE_MILLISECONDS / numKeyframes;
  unsigned long now = millis();
  
  if ((now - lastBreath) > period) {
    lastBreath = now;

    for (int i = 0; i < strip.numPixels(); i++) {
      uint8_t color = (127 * KEYFRAMES[keyframePointer]) / 256;
      strip.setPixelColor(i, color, 0, 0);
    }
    strip.show();   

    // Increment the keyframe pointer.
    if (++keyframePointer > numKeyframes) {
      // Reset to 0 after the last keyframe.
      keyframePointer = 0;
    }   
  }
}
