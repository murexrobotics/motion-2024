// Firmware for Servo Pico

#include <Servo.h>
#include <Arduino.h>

// ------ DO NOT CHANGE THESE VALUES -------
#define S1_PWM_PIN 3
#define S2_PWM_PIN 5

#define ALL_CALL_ADDRESS 48

// -- Update for each individual thruster --
#define ADDRESS 57
#define ADDRESS2 58

// ---- Uncomment to enable debug mode -----
#define DEBUG

Servo servo1;
Servo servo2;

void setup() {
  /// ANYESC MASCP CODE
  delay(5000);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(25, OUTPUT); // Regular Pico Support

  // Initialize UART
  Serial1.setRX(13); // default: 13, baackup: 5
  Serial1.setTX(12); // default: 12, backup: 4
  Serial1.begin(115200);
  Serial.begin(115200);

  while (!Serial1);
  
  // Initialize thruster
  #ifdef DEBUG 
    Serial.print("Initializing Thruster ");
    Serial.print(ADDRESS - 48);
    Serial.print("\n");
  #endif

  servo1.attach(S1_PWM_PIN);
  servo1.writeMicroseconds(1060);
  servo2.attach(S2_PWM_PIN);
  servo2.writeMicroseconds(1000);

  servo2.attach(S2_PWM_PIN);
  // delay(7000); // wait for ESC to initialize

  #ifdef DEBUG
    Serial.print("Finished Initializing Thruster");
  #endif


  // Turn on LED to indicate finished initialization
  digitalWrite(0, HIGH);
  digitalWrite(25, HIGH); // Regular Pico Support
}

void loop() {
  if (Serial1.available() == 2) {
    // Turn on LED to indicate that UART is working
    digitalWrite(1, HIGH);

    int address = Serial1.read();
    int throttle = Serial1.read();

    #ifdef DEBUG
      Serial.print("\n");
      Serial.print(address);
      Serial.print(" ");
      Serial.print(throttle);
      Serial.print(" ");
    #endif

    if (address == ADDRESS) {
      // Turn on LED to indicate that MASCP is working
      digitalWrite(2, HIGH);
      
      int pulse = map(throttle, 0, 255, 500, 2500);
      // if (throttle == 127) { pulse = 1500; } // Deadzone

      #ifdef DEBUG
        Serial.print(pulse);
      #endif

      servo1.writeMicroseconds(pulse);
    } else if (address == ADDRESS2) {
      // Turn on LED to indicate that MASCP is working
      digitalWrite(2, HIGH);
      int pulse = map(throttle, 0, 255, 500, 2500);

      #ifdef DEBUG
        Serial.print(pulse);
      #endif

      servo2.writeMicroseconds(pulse);
    }
  }
}