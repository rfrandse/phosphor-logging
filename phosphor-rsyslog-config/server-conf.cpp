#include "server-conf.hpp"
#include "utils.hpp"
#include <fstream>

namespace phosphor
{
namespace rsyslog_config
{

namespace utils = phosphor::rsyslog_utils;

std::string Server::address(std::string value)
{
    auto serverAddress = address();
    if (serverAddress == value)
    {
        return serverAddress;
    }

    writeConfig(value, port(), configFilePath.c_str());
    auto result = NetworkClient::address(value);
    return result;
}

uint16_t Server::port(uint16_t value)
{
    auto serverPort = port();
    if (serverPort == value)
    {
        return serverPort;
    }

    writeConfig(address(), value, configFilePath.c_str());
    auto result = NetworkClient::port(value);
    return result;
}

void Server::writeConfig(
                 const std::string& serverAddress,
                 uint16_t serverPort,
                 const char* filePath)
{
    std::fstream stream(filePath, std::fstream::out);

    if (serverPort && !serverAddress.empty())
    {
        // write '*.* @@<remote-host>:<port>'
        stream << "*.* @@" << serverAddress << ":" << serverPort;
    }
    else // this is a disable request
    {
        // write '#*.* @@remote-host:port'
        stream << "#*.* @@remote-host:port";
    }

    utils::restart();
}

} // namespace rsyslog_config
} // namespace phosphor