#include "DHTesp.h" 
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

const int id = ESP.getChipId();
const char* ssid = "ssid"; 
const char* password = "password";
const char* host = "http://192.168.0.200:9000";
String url = "/readings/tt/";

DHTesp dht;

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(5000);
  
  dht.setup(5, DHTesp::DHT11); // Connect DHT sensor to GPIO 5
  Serial.println(WiFi.localIP());

  WiFiClient client;
  
  delay(dht.getMinimumSamplingPeriod());
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  String sensorStatus = dht.getStatusString();
  
  Serial.print(sensorStatus);
  Serial.print("\t");
  Serial.print("Humidity: ");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print("Temperature: ");
  Serial.print(temperature, 1);
  Serial.print("\n");

  String postData = "Status: " + sensorStatus + " Temperature: " + String(temperature) + " Humidity: " + String(humidity) + " ID: " + String(id);  
  String address = host + url;
  Serial.println(address);
  Serial.println(postData);
  
  HTTPClient http;
  http.begin(client, address);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  auto httpCode = http.POST(postData);
  Serial.println(httpCode);
  String payload = http.getString(); 
  Serial.println(payload);
  http.end();

  delay(10);
  Serial.println("Entering Deep Sleep");
  ESP.deepSleep(60000000);
  delay(10);
}

void loop()
{
  }
