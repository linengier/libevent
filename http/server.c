#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void generic_cb(struct evhttp_request* req, void* arg)
{
    printf("get generic\r\n");
	struct evbuffer* buffer = evbuffer_new();
	char* s = "This is the generic buf";
	evbuffer_add(buffer, s, strlen(s));
	evhttp_send_reply(req, 200, "OK", buffer);
	evbuffer_free(buffer);
}

void test_cb(struct evhttp_request* req, void* arg)
{
	struct evbuffer* buffer = evbuffer_new();
	char* s = "This is the test buf";
    printf("get test call\r\n");
	evbuffer_add(buffer, s, strlen(s));
	evbuffer_add(buffer, (char*)arg, strlen(arg));
	evhttp_send_reply(req, 200, "OK", buffer);
	evbuffer_free(buffer);
}


int main()
{
	short http_port = 8081;
	char* http_addr = "0.0.0.0";

	struct event_base* base = event_base_new();
	struct evhttp* http_server = evhttp_new(base);
	if(NULL == http_server)
	{
		return -1;
	}

	int ret = evhttp_bind_socket(http_server, http_addr, http_port);
	if(ret != 0)
	{
		return -1;
	}

	evhttp_set_cb(http_server, "/test", test_cb, "arg");
	evhttp_set_gencb(http_server, generic_cb, NULL);
	printf("http server start OK!\n");
	event_base_dispatch(base);
	evhttp_free(http_server);

	return 0;
}
