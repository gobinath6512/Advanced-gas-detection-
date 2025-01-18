#include <SoftwareSerial.h>

// MQ2 sensor
const int gasSensorPin = A0;
int sensorValue = 0;
const int threshold = 200; // Adjust based on your environment

// GSM Module
SoftwareSerial gsm(7, 8); // TX, RX

// Buzzer
const int buzzerPin = 13;

void setup() {
  Serial.begin(9600);    // Serial Monitor
  gsm.begin(9600);       // GSM module baud rate
  
  pinMode(gasSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // Initialize GSM module
  initializeGSM();
}

void loop() {
  sensorValue = analogRead(gasSensorPin);
  Serial.print("Gas Sensor Value: ");
  Serial.println(sensorValue);
  
  if (sensorValue > threshold) {
    Serial.println("Harmful Gas Detected!");
    
    // Turn on the buzzer
    digitalWrite(buzzerPin, HIGH);
    
    // Send SMS alert
    sendSMS("Alert: Harmful Gas Detected !");
    
    delay(5000); // Wait for 5 seconds before next check
  } else {
    // Turn off the buzzer
    digitalWrite(buzzerPin, LOW);
  }
  
  delay(500); // Sensor check every second
}

void initializeGSM() {
  delay(1000);
  gsm.println("AT");           // Check if GSM module is responding
  delay(100);
  gsm.println("AT+CMGF=1");    // Set SMS to text mode
  delay(100);
  gsm.println("AT+CNMI=1,2,0,0,0"); // Configure for SMS reception
  delay(100);
  Serial.println("GSM Module Initialized.");
}

void sendSMS(String message) {
  gsm.println("AT+CMGS=\"+917395809454\""); // Replace with your phone number
  delay(100);
  
  gsm.println(message);
  delay(100);
  
  gsm.write((char)26); // ASCII code of Ctrl+Z to send SMS
  delay(1000);
  
  Serial.println("SMS sent successfully.");
}
