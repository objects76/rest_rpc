
#pragma once

#include <string>
#include <stdarg.h>


#ifdef _WIN32
#define pathsep		('\\')
#else
#define pathsep		('/')
inline int GetModuleFileNameA(void* handle, char* buf, size_t bufSize) {
	return readlink("/proc/self/exe", buf, bufSize);
}
#endif

inline std::string csprintf(const char* fmt, ...)
{
	char buffer[4096];
	va_list args;
	va_start(args, fmt);
	int n = vsprintf_s(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	auto str = std::string(buffer, n);
	return str;
}

#define LOG(...)	log(__FILE__, __LINE__, __VA_ARGS__)
inline void log(const char* file, int line, const char* fmt, ...)
{
	int n = 0;
	char buffer[4096];

	auto p = file ? strrchr(file, pathsep) : nullptr;
	if (p) 
		file = p + 1;

	n += snprintf(buffer + n, sizeof(buffer) - n, "%d.%-8d ", GetCurrentProcessId(), GetCurrentThreadId());

	va_list args;
	va_start(args, fmt);
	n += vsprintf_s(buffer + n, sizeof(buffer) - n, fmt, args);
	va_end(args);

	if (file)
		snprintf(buffer + n, sizeof(buffer) - n, " at %s:%d\n", file, line);
	else
		strcpy(buffer + n, "\n");

	::OutputDebugStringA(buffer);
	std::cout << buffer;
}

#ifndef CONCAT
#define CONCAT_(X,Y)				X##Y
#define CONCAT(X,Y)					CONCAT_(X,Y)
#define TOSTR2(x)					#x
#define TOSTR(x)					TOSTR2(x)
#endif

#define FNSCOPE() fnscope CONCAT(fn190604,__LINE__)(__FUNCTION__)
struct fnscope {
	const char* msg_;
	fnscope(const char* msg) : msg_(msg) {
		log(nullptr, 0, "{ %s", msg_);
	}
	~fnscope() {
		log(nullptr, 0, "} %s", msg_);
	}
};


inline std::string get_executable_path()
{
	char path[512];
	GetModuleFileNameA(nullptr, path, sizeof(path));
	return path;
}

