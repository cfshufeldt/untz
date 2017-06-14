/*************************************************** 
 Single Trellis board to play tones
 ****************************************************/

#include <Wire.h>
#include "Adafruit_Trellis.h"

/*************************************************** 
  This example shows reading buttons and setting/clearing buttons in a loop
  "momentary" mode has the LED light up only when a button is pressed
  "latching" mode lets you turn the LED on/off when pressed

  Up to 8 matrices can be used but this example will show 4 or 1
 ****************************************************/

Adafruit_Trellis matrix0 = Adafruit_Trellis();

// uncomment the below to add 3 more matrices
/*
Adafruit_Trellis matrix1 = Adafruit_Trellis();
Adafruit_Trellis matrix2 = Adafruit_Trellis();
Adafruit_Trellis matrix3 = Adafruit_Trellis();
// you can add another 4, up to 8
*/

// Just one
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);
// or use the below to select 4, up to 8 can be passed in
//Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0, &matrix1, &matrix2, &matrix3);

// set to however many you're working with here, up to 8
#define NUMTRELLIS 1

#define numKeys (NUMTRELLIS * 16)

// Connect Trellis Vin to 5V and Ground to ground.
// Connect the INT wire to pin #A2 (can change later!)
#define INTPIN A2
// Connect I2C SDA pin to your Arduino SDA line
// Connect I2C SCL pin to your Arduino SCL line
// All Trellises share the SDA, SCL and INT pin! 
// Even 8 tiles use only 3 wires max

int speakerPin = 11;

int numTones = 16;
int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 554, 587, 622};
//            mid C  C#   D    D#   E    F    F#   G    G#   A


void setup() {
  Serial.begin(9600);
  Serial.println("Trellis Demo");

  // INT pin requires a pullup
  pinMode(INTPIN, INPUT);
  digitalWrite(INTPIN, HIGH);
  
  // begin() with the addresses of each panel in order
  // I find it easiest if the addresses are in order
  trellis.begin(0x70);  // only one
  // trellis.begin(0x70, 0x71, 0x72, 0x73);  // or four!

  // light up all the LEDs in order
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.setLED(i);
    tone(speakerPin, tones[i]);
    trellis.writeDisplay();    
    delay(50);
  }
  
  noTone(speakerPin);

  // then turn them off
  for (uint8_t i=numKeys+1; i>0; i--) {
    trellis.clrLED(i-1);
    tone(speakerPin, tones[i-1]);
    trellis.writeDisplay();    
    delay(50);
  }

  noTone(speakerPin);

}


void loop() {
  delay(30); // 30ms delay is required, dont remove me!
  
    // If a button was just pressed or released...
    if (trellis.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
	// if it was pressed, turn it on
	if (trellis.justPressed(i)) {
	  Serial.print("v"); Serial.println(i);
	  trellis.setLED(i);
    tone(speakerPin, tones[i]);
	} 
	// if it was released, turn it off
	if (trellis.justReleased(i)) {
	  Serial.print("^"); Serial.println(i);
	  trellis.clrLED(i);
    noTone(speakerPin);
	}
      }
      // tell the trellis to set the LEDs we requested
      trellis.writeDisplay();
    }

}

