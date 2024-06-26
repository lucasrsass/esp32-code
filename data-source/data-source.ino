#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

const char* ssid = "ACTX_Corner-Office"; // Replace with your actual SSID
const char* password = "falcon2023"; // Replace with your actual password
const char* serverAddress = "192.168.68.196"; // Replace with your actual server IP address

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("Disconnected");
      break;
    case WStype_CONNECTED:
      Serial.println("Connected");
      break;
    case WStype_TEXT:
      Serial.printf("Received text: %s\n", payload);
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  webSocket.begin(serverAddress, 4000, "/");
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();

  static unsigned long lastSendTime = 0;
  if (millis() - lastSendTime > 5000) {
    lastSendTime = millis();

    DynamicJsonDocument doc(1024);
    doc["device"] = "esp32_1";
    doc["value"] = 123.45;

    String data;
    serializeJson(doc, data);

    webSocket.sendTXT(data);
    Serial.println("Data sent: " + data);
  }
}
