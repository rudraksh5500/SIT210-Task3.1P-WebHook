#include <DHT.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>

#define DHTPIN 2
#define DHTTYPE DHT11

char ssid[] = "Redmi";             // WiFi SSID
char pass[] = "55775577";                 // WiFi Password

unsigned long channelID = 2250597;        // my ThingSpeak Channel ID
const char *apiKey = "FKY18BD1AUBKM83T";  // my ThingSpeak API Key

DHT dht(DHTPIN, DHTTYPE);                
WiFiClient client;                       

void setup() {
  Serial.begin(9600);                     
  dht.begin();                            // Initialize DHT sensor
  establishWiFiConnection();              // Connect to WiFi
  initializeThingSpeak();                 // Initialize ThingSpeak
}

void loop() {
  delay(2000);                            // Delay between sensor readings

  float temperature = dht.readTemperature();     // Read temperature in Celsius
  float humidity = dht.readHumidity();           // Read humidity

  if (!isnan(temperature) && !isnan(humidity)) {
    sendSensorDataToThingSpeak(temperature, humidity);  // Send data to ThingSpeak
  } else {
    Serial.println("Error reading data from DHT sensor");
  }
  
  delay(30000);                           // Delay before the next reading
}

void establishWiFiConnection() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, pass);
    delay(1000);
  }
  Serial.println("Connected to WiFi");
}

void initializeThingSpeak() {
  ThingSpeak.begin(client);              
}

void sendSensorDataToThingSpeak(float temp, float humidity) {
  ThingSpeak.setField(1, temp);           // Set temperature data in Field 1
  ThingSpeak.setField(2, humidity);       // Set humidity data in Field 2
  
  int responseCode = ThingSpeak.writeFields(channelID, apiKey); // Send data

  if (responseCode == 200) {
    Serial.println("Data successfully sent to ThingSpeak");
  } else {
    Serial.println("Error sending data to ThingSpeak");
  }
}
