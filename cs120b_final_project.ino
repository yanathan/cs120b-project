#include "SevSeg.h"
#include "DHT.h"
#include <TimeLib.h>
#define DHTPIN A3   
#define DHTTYPE DHT11   // DHT 11 

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);
SevSeg sevseg;

const int numDigits = 4;
const byte digitPins[] = {4, 11, 12, 7};
const byte segmentPins[] = {9, 13, 5, 3, 2, 10, 6, 8};

// Joystick pins
const int joyXPin = A0; 
const int joyYPin = A1; 

enum Mode { TEMPERATURE_F, TEMPERATURE_C, HUMIDITY, TIME };
Mode currentMode = TEMPERATURE_F;

unsigned long displayTimer = 0; // Timer to control display refresh


void setup() {
  Serial.begin(9600);
  delay(2000);
  dht.begin();

  byte hardwareConfig = COMMON_CATHODE;
  bool resistorsOnSegments = false;
  bool updateWithDelays = false;
  bool leadingZeros = false;
  bool disableDecPoint = false;
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.refreshDisplay();
}

void displayTemperatureF() {
  int temperatureF = dht.readTemperature(true); // Read temperature in Fahrenheit
  //sevseg.setNumber(temperatureF);
  String temperatureString = String(temperatureF); 
  temperatureString.concat("F"); 
char charArray[temperatureString.length() + 1]; // +1 for the null terminator
// Convert the String to a char array
temperatureString.toCharArray(charArray, temperatureString.length() + 1);
  sevseg.setChars(charArray);
  sevseg.refreshDisplay();
}

void displayTemperatureC() {
  int temperatureC = dht.readTemperature(); // Read temperature in Celsius
  //sevseg.setNumber(temperatureF);
  String temperatureString = String(temperatureC); 
  temperatureString.concat("C"); 
char charArray[temperatureString.length() + 1]; // +1 for the null terminator
// Convert the String to a char array
temperatureString.toCharArray(charArray, temperatureString.length() + 1);
  sevseg.setChars(charArray);
  sevseg.refreshDisplay();
}

void displayHumidity() {
  int humidity = dht.readHumidity(); 
  //sevseg.setNumber(temperatureF);
  String humidityString = String(humidity); 
  humidityString.concat("H"); 
  char charArray[humidityString.length() + 1]; // +1 for the null terminator
  // Convert the String to a char array
  humidityString.toCharArray(charArray, humidityString.length() + 1);
  sevseg.setChars(charArray);
  sevseg.refreshDisplay();
}

void displayCurrentTime() {
  int hours = hour();
  int minutes = minute();

  // Convert hours and minutes to characters with proper ASCII values
  char charArray[] = {
    (hours / 10) + '0', (hours % 10) + '0',
    (minutes / 10) + '0', (minutes % 10) + '0'
  };

  // Set the characters on the display and refresh it
  sevseg.setChars(charArray);
  sevseg.refreshDisplay();
}



void loop() {
 int joyXVal = analogRead(joyXPin);

  if (joyXVal < 100) {
    currentMode = TEMPERATURE_F;
  } else if (joyXVal > 900) {
    currentMode = HUMIDITY;
  }
    else {
    currentMode = TEMPERATURE_C;
  }

  switch (currentMode) {
    case TEMPERATURE_F:
      displayTemperatureF();
      break;

    case TEMPERATURE_C:
      displayTemperatureC();
      break;

    case HUMIDITY:
      displayHumidity();
      break;

  }
    /*float f = dht.readTemperature(true); // Read temperature in Fahrenheit
    float h = dht.readHumidity(); // Read temperature in Fahrenheit
    float c = dht.readTemperature(); // Read temperature in Fahrenheit

    if (isnan(h) || isnan(c) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }


  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(c);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.println(" *F");
  */
  // Refresh display every 100 milliseconds (adjust as needed)
  if (millis() - displayTimer >= 100) {
    sevseg.refreshDisplay();
    displayTimer = millis();
  }
}
