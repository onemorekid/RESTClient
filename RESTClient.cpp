// RESTClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include"curl/curl.h"
#include<sstream>

class Response {
private:
    std::string contentType;
    std::string data;
public:
    Response(std::string strType, std::string strData) {
        contentType = strType;
        data = strData;
    }

    std::string GetData() {
        return data;
    }

    std::string GetContentType() {
        return contentType;
    }
};
class RESTClient {
private:
    static CURL* handle;
public:
    static Response GET(const char* url) {
        handle = curl_easy_init();
        curl_easy_setopt(handle, CURLOPT_URL, url);
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
        std::stringstream ss;
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&ss);
        auto res = curl_easy_perform(handle);
        std::string contentType = "Unknown";
        if (CURLE_OK == res) {
            char* ct;
            res = curl_easy_getinfo(handle, CURLINFO_CONTENT_TYPE, &ct);
            contentType = ct;
        }
        return Response(contentType, ss.str());
    }

    static size_t write_callback(void* source_p, size_t size, size_t nmemb, void* dest_p) {
        auto realsize = size * nmemb;
        std::string chunk((char*)source_p, realsize);
        *((std::stringstream*)dest_p) << chunk;
        return realsize;
    }

};
CURL* RESTClient::handle;


int main()
{
    curl_global_init(CURL_GLOBAL_ALL);
    std::cout << RESTClient::GET("http://www.google.com").GetData();
    curl_global_cleanup();
}

