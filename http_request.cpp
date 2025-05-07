#include <iostream>
#include <string>
#include <curl/curl.h>

// Función de escritura para guardar la respuesta de cURL
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Función para descargar los datos de Yahoo Finance
std::string download_data(const std::string& url) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::cerr << "cURL request failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return readBuffer;
}

int main() {
    std::string ticker = "AAPL";  // Acción a consultar (Ejemplo: Apple)
    std::string url = "https://query1.finance.yahoo.com/v7/finance/download/" + ticker + 
                     "?period1=1500000000&period2=1500000000&interval=1d&events=history";

    std::string data = download_data(url);
    std::cout << data << std::endl;  // Imprimir los datos obtenidos

    return 0;
}
