/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 * Generic Security Service Application Program Interface (GSSAPI)
 *
 * Copyright 2015 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <winpr/crt.h>
#include <winpr/library.h>

#include "sspi_gss.h"

#include "../../log.h"
#define TAG WINPR_TAG("sspi.gss")

int sspi_GssApiInit();

GSSAPI_FUNCTION_TABLE g_GssApi;
static BOOL g_Initialized = FALSE;
static HMODULE g_GssApiModule = NULL;

#define GSSAPI_STUB_CALL(_name, ...) \
	if (!g_Initialized) \
		sspi_GssApiInit(); \
	if (!g_GssApi.gss_ ## _name) \
		return 0; \
	return g_GssApi.gss_ ## _name ( __VA_ARGS__ )

/**
 * SSPI GSSAPI OIDs
 */

static sspi_gss_OID_desc g_SSPI_GSS_C_NT_USER_NAME =		{ 10, (void*) "\x2a\x86\x48\x86\xf7\x12\x01\x02\x01\x01" };
static sspi_gss_OID_desc g_SSPI_GSS_C_NT_MACHINE_UID_NAME =	{ 10, (void*) "\x2a\x86\x48\x86\xf7\x12\x01\x02\x01\x02" };
static sspi_gss_OID_desc g_SSPI_GSS_C_NT_STRING_UID_NAME =	{ 10, (void*) "\x2a\x86\x48\x86\xf7\x12\x01\x02\x01\x03" };
static sspi_gss_OID_desc g_SSPI_GSS_C_NT_HOSTBASED_SERVICE_X =	{ 6,  (void*) "\x2b\x06\x01\x05\x06\x02" };
static sspi_gss_OID_desc g_SSPI_GSS_C_NT_HOSTBASED_SERVICE =	{ 10, (void*) "\x2a\x86\x48\x86\xf7\x12\x01\x02\x01\x04" };
static sspi_gss_OID_desc g_SSPI_GSS_C_NT_ANONYMOUS =		{ 6,  (void*) "\x2b\x06\01\x05\x06\x03" };
static sspi_gss_OID_desc g_SSPI_GSS_C_NT_EXPORT_NAME =		{ 6,  (void*) "\x2b\x06\x01\x05\x06\x04" };

sspi_gss_OID SSPI_GSS_C_NT_USER_NAME = &g_SSPI_GSS_C_NT_USER_NAME;
sspi_gss_OID SSPI_GSS_C_NT_MACHINE_UID_NAME = &g_SSPI_GSS_C_NT_MACHINE_UID_NAME;
sspi_gss_OID SSPI_GSS_C_NT_STRING_UID_NAME = &g_SSPI_GSS_C_NT_STRING_UID_NAME;
sspi_gss_OID SSPI_GSS_C_NT_HOSTBASED_SERVICE_X = &g_SSPI_GSS_C_NT_HOSTBASED_SERVICE_X;
sspi_gss_OID SSPI_GSS_C_NT_HOSTBASED_SERVICE = &g_SSPI_GSS_C_NT_HOSTBASED_SERVICE;
sspi_gss_OID SSPI_GSS_C_NT_ANONYMOUS = &g_SSPI_GSS_C_NT_ANONYMOUS;
sspi_gss_OID SSPI_GSS_C_NT_EXPORT_NAME = &g_SSPI_GSS_C_NT_EXPORT_NAME;

/**
 * SSPI GSSAPI Wrapper Stubs
 */

UINT32 SSPI_GSSAPI sspi_gss_acquire_cred(
	UINT32* minor_status,
	sspi_gss_name_t desired_name,
	UINT32 time_req,
	sspi_gss_OID_set desired_mechs,
	sspi_gss_cred_usage_t cred_usage,
	sspi_gss_cred_id_t* output_cred_handle,
	sspi_gss_OID_set* actual_mechs,
	UINT32* time_rec)
{
	GSSAPI_STUB_CALL(acquire_cred, minor_status, desired_name, time_req,
			desired_mechs, cred_usage, output_cred_handle, actual_mechs, time_rec);
}

UINT32 SSPI_GSSAPI sspi_gss_release_cred(
	UINT32* minor_status,
	sspi_gss_cred_id_t* cred_handle)
{
	GSSAPI_STUB_CALL(release_cred, minor_status, cred_handle);
}

UINT32 SSPI_GSSAPI sspi_gss_init_sec_context(
	UINT32* minor_status,
	sspi_gss_cred_id_t claimant_cred_handle,
	sspi_gss_ctx_id_t* context_handle,
	sspi_gss_name_t target_name,
	sspi_gss_OID mech_type,
	UINT32 req_flags,
	UINT32 time_req,
	sspi_gss_channel_bindings_t input_chan_bindings,
	sspi_gss_buffer_t input_token,
	sspi_gss_OID* actual_mech_type,
	sspi_gss_buffer_t output_token,
	UINT32* ret_flags,
	UINT32* time_rec)
{
	GSSAPI_STUB_CALL(init_sec_context, minor_status, claimant_cred_handle, context_handle,
			target_name, mech_type, req_flags, time_req, input_chan_bindings,
			input_token, actual_mech_type, output_token, ret_flags, time_rec);
}

UINT32 SSPI_GSSAPI sspi_gss_accept_sec_context(
	UINT32* minor_status,
	sspi_gss_ctx_id_t* context_handle,
	sspi_gss_cred_id_t acceptor_cred_handle,
	sspi_gss_buffer_t input_token_buffer,
	sspi_gss_channel_bindings_t input_chan_bindings,
	sspi_gss_name_t* src_name,
	sspi_gss_OID* mech_type,
	sspi_gss_buffer_t output_token,
	UINT32* ret_flags,
	UINT32* time_rec,
	sspi_gss_cred_id_t* delegated_cred_handle)
{
	GSSAPI_STUB_CALL(accept_sec_context, minor_status, context_handle, acceptor_cred_handle,
			input_token_buffer, input_chan_bindings, src_name, mech_type, output_token,
			ret_flags, time_rec, delegated_cred_handle);
}

UINT32 SSPI_GSSAPI sspi_gss_process_context_token(
	UINT32* minor_status,
	sspi_gss_ctx_id_t context_handle,
	sspi_gss_buffer_t token_buffer)
{
	GSSAPI_STUB_CALL(process_context_token, minor_status, context_handle, token_buffer);
}

UINT32 SSPI_GSSAPI sspi_gss_delete_sec_context(
	UINT32* minor_status,
	sspi_gss_ctx_id_t* context_handle,
	sspi_gss_buffer_t output_token)
{
	GSSAPI_STUB_CALL(delete_sec_context, minor_status, context_handle, output_token);
}

UINT32 SSPI_GSSAPI sspi_gss_context_time(
	UINT32* minor_status,
	sspi_gss_ctx_id_t context_handle,
	UINT32* time_rec)
{
	GSSAPI_STUB_CALL(context_time, minor_status, context_handle, time_rec);
}

UINT32 SSPI_GSSAPI sspi_gss_get_mic(
	UINT32* minor_status,
	sspi_gss_ctx_id_t context_handle,
	sspi_gss_qop_t qop_req,
	sspi_gss_buffer_t message_buffer,
	sspi_gss_buffer_t message_token)
{
	GSSAPI_STUB_CALL(get_mic, minor_status, context_handle, qop_req, message_buffer, message_token);
}

UINT32 SSPI_GSSAPI sspi_gss_verify_mic(
	UINT32* minor_status,
	sspi_gss_ctx_id_t context_handle,
	sspi_gss_buffer_t message_buffer,
	sspi_gss_buffer_t message_token,
	sspi_gss_qop_t* qop_state)
{
	GSSAPI_STUB_CALL(verify_mic, minor_status, context_handle, message_buffer, message_token, qop_state);
}

UINT32 SSPI_GSSAPI sspi_gss_wrap(
	UINT32* minor_status,
	sspi_gss_ctx_id_t context_handle,
	int conf_req_flag,
	sspi_gss_qop_t qop_req,
	sspi_gss_buffer_t input_message_buffer,
	int* conf_state,
	sspi_gss_buffer_t output_message_buffer)
{
	GSSAPI_STUB_CALL(wrap, minor_status, context_handle, conf_req_flag,
			qop_req, input_message_buffer, conf_state, output_message_buffer);
}

UINT32 SSPI_GSSAPI sspi_gss_unwrap(
	UINT32* minor_status,
	sspi_gss_ctx_id_t context_handle,
	sspi_gss_buffer_t input_message_buffer,
	sspi_gss_buffer_t output_message_buffer,
	int* conf_state,
	sspi_gss_qop_t* qop_state)
{
	GSSAPI_STUB_CALL(unwrap, minor_status, context_handle, input_message_buffer,
			output_message_buffer, conf_state, qop_state);
}

UINT32 SSPI_GSSAPI sspi_gss_display_status(
	UINT32* minor_status,
	UINT32 status_value,
	int status_type,
	sspi_gss_OID mech_type,
	UINT32* message_context,
	sspi_gss_buffer_t status_string)
{
	GSSAPI_STUB_CALL(display_status, minor_status, status_value, status_type,
			mech_type, message_context, status_string);
}

UINT32 SSPI_GSSAPI sspi_gss_indicate_mechs(
	UINT32* minor_status,
	sspi_gss_OID_set* mech_set)
{
	GSSAPI_STUB_CALL(indicate_mechs, minor_status, mech_set);
}

UINT32 SSPI_GSSAPI sspi_gss_compare_name(
	UINT32* minor_status,
	sspi_gss_name_t name1,
	sspi_gss_name_t name2,
	int* name_equal)
{
	GSSAPI_STUB_CALL(compare_name, minor_status, name1, name2, name_equal);
}

UINT32 SSPI_GSSAPI sspi_gss_display_name(
	UINT32* minor_status,
	sspi_gss_name_t input_name,
	sspi_gss_buffer_t output_name_buffer,
	sspi_gss_OID* output_name_type)
{
	GSSAPI_STUB_CALL(display_name, minor_status, input_name, output_name_buffer, output_name_type);
}

UINT32 SSPI_GSSAPI sspi_gss_import_name(
	UINT32* minor_status,
	sspi_gss_buffer_t input_name_buffer,
	sspi_gss_OID input_name_type,
	sspi_gss_name_t* output_name)
{
	GSSAPI_STUB_CALL(import_name, minor_status, input_name_buffer, input_name_type, output_name);
}

UINT32 SSPI_GSSAPI sspi_gss_release_name(
	UINT32* minor_status,
	sspi_gss_name_t* input_name)
{
	GSSAPI_STUB_CALL(release_name, minor_status, input_name);
}

UINT32 SSPI_GSSAPI sspi_gss_release_buffer(
	UINT32* minor_status,
	sspi_gss_buffer_t buffer)
{
	GSSAPI_STUB_CALL(release_buffer, minor_status, buffer);
}

UINT32 SSPI_GSSAPI sspi_gss_release_oid_set(
	UINT32* minor_status,
	sspi_gss_OID_set* set)
{
	GSSAPI_STUB_CALL(release_oid_set, minor_status, set);
}

UINT32 SSPI_GSSAPI sspi_gss_inquire_cred(
	UINT32* minor_status,
	sspi_gss_cred_id_t cred_handle,
	sspi_gss_name_t* name,
	UINT32* lifetime,
	sspi_gss_cred_usage_t* cred_usage,
	sspi_gss_OID_set* mechanisms)
{
	GSSAPI_STUB_CALL(inquire_cred, minor_status, cred_handle, name, lifetime, cred_usage, mechanisms);
}

UINT32 SSPI_GSSAPI sspi_gss_inquire_context(
	UINT32* minor_status,
	sspi_gss_ctx_id_t context_handle,
	sspi_gss_name_t* src_name,
	sspi_gss_name_t* targ_name,
	UINT32* lifetime_rec,
	sspi_gss_OID* mech_type,
	UINT32* ctx_flags,
	int* locally_initiated,
	int* open)
{
	GSSAPI_STUB_CALL(inquire_context, minor_status, context_handle, src_name, targ_name,
			lifetime_rec, mech_type, ctx_flags, locally_initiated, open);
}

UINT32 SSPI_GSSAPI sspi_gss_wrap_size_limit(
	UINT32* minor_status,
	sspi_gss_ctx_id_t context_handle,
	int conf_req_flag,
	sspi_gss_qop_t qop_req,
	UINT32 req_output_size,
	UINT32* max_input_size)
{
	GSSAPI_STUB_CALL(wrap_size_limit, minor_status, context_handle,
			conf_req_flag, qop_req, req_output_size, max_input_size);
}

UINT32 SSPI_GSSAPI sspi_gss_import_name_object(
	UINT32* minor_status,
	void* input_name,
	sspi_gss_OID input_name_type,
	sspi_gss_name_t* output_name)
{
	GSSAPI_STUB_CALL(import_name_object, minor_status, input_name, input_name_type, output_name);
}

UINT32 SSPI_GSSAPI sspi_gss_export_name_object(
	UINT32* minor_status,
	sspi_gss_name_t input_name,
	sspi_gss_OID desired_name_type,
	void** output_name)
{
	GSSAPI_STUB_CALL(export_name_object, minor_status, input_name, desired_name_type, output_name);
}

UINT32 SSPI_GSSAPI sspi_gss_add_cred(
	UINT32* minor_status,
	sspi_gss_cred_id_t input_cred_handle,
	sspi_gss_name_t desired_name,
	sspi_gss_OID desired_mech,
	sspi_gss_cred_usage_t cred_usage,
	UINT32 initiator_time_req,
	UINT32 acceptor_time_req,
	sspi_gss_cred_id_t* output_cred_handle,
	sspi_gss_OID_set* actual_mechs,
	UINT32* initiator_time_rec,
	UINT32* acceptor_time_rec)
{
	GSSAPI_STUB_CALL(add_cred, minor_status, input_cred_handle, desired_name, desired_mech, cred_usage,
			initiator_time_req, acceptor_time_req, output_cred_handle, actual_mechs, initiator_time_rec, acceptor_time_rec);
}

UINT32 SSPI_GSSAPI sspi_gss_inquire_cred_by_mech(
	UINT32* minor_status,
	sspi_gss_cred_id_t cred_handle,
	sspi_gss_OID mech_type,
	sspi_gss_name_t* name,
	UINT32* initiator_lifetime,
	UINT32* acceptor_lifetime,
	sspi_gss_cred_usage_t* cred_usage)
{
	GSSAPI_STUB_CALL(inquire_cred_by_mech, minor_status, cred_handle, mech_type, name,
			initiator_lifetime, acceptor_lifetime, cred_usage);
}

UINT32 SSPI_GSSAPI sspi_gss_export_sec_context(
	UINT32* minor_status,
	sspi_gss_ctx_id_t* context_handle,
	sspi_gss_buffer_t interprocess_token)
{
	GSSAPI_STUB_CALL(export_sec_context, minor_status, context_handle, interprocess_token);
}

UINT32 SSPI_GSSAPI sspi_gss_import_sec_context(
	UINT32* minor_status,
	sspi_gss_buffer_t interprocess_token,
	sspi_gss_ctx_id_t* context_handle)
{
	GSSAPI_STUB_CALL(import_sec_context, minor_status, interprocess_token, context_handle);
}

UINT32 SSPI_GSSAPI sspi_gss_release_oid(
	UINT32* minor_status,
	sspi_gss_OID* oid)
{
	GSSAPI_STUB_CALL(release_oid, minor_status, oid);
}

UINT32 SSPI_GSSAPI sspi_gss_create_empty_oid_set(
	UINT32* minor_status,
	sspi_gss_OID_set* oid_set)
{
	GSSAPI_STUB_CALL(create_empty_oid_set, minor_status, oid_set);
}

UINT32 SSPI_GSSAPI sspi_gss_add_oid_set_member(
	UINT32* minor_status,
	sspi_gss_OID member_oid,
	sspi_gss_OID_set* oid_set)
{
	GSSAPI_STUB_CALL(add_oid_set_member, minor_status, member_oid, oid_set);
}

UINT32 SSPI_GSSAPI sspi_gss_test_oid_set_member(
	UINT32* minor_status,
	sspi_gss_OID member,
	sspi_gss_OID_set set,
	int* present)
{
	GSSAPI_STUB_CALL(test_oid_set_member, minor_status, member, set, present);
}

UINT32 SSPI_GSSAPI sspi_gss_str_to_oid(
	UINT32* minor_status,
	sspi_gss_buffer_t oid_str,
	sspi_gss_OID* oid)
{
	GSSAPI_STUB_CALL(str_to_oid, minor_status, oid_str, oid);
}

UINT32 SSPI_GSSAPI sspi_gss_oid_to_str(
	UINT32* minor_status,
	sspi_gss_OID oid,
	sspi_gss_buffer_t oid_str)
{
	GSSAPI_STUB_CALL(oid_to_str, minor_status, oid, oid_str);
}

UINT32 SSPI_GSSAPI sspi_gss_inquire_names_for_mech(
	UINT32* minor_status,
	sspi_gss_OID mechanism,
	sspi_gss_OID_set* name_types)
{
	GSSAPI_STUB_CALL(inquire_names_for_mech, minor_status, mechanism, name_types);
}

UINT32 SSPI_GSSAPI sspi_gss_inquire_mechs_for_name(
	UINT32* minor_status,
	const sspi_gss_name_t input_name,
	sspi_gss_OID_set* mech_types)
{
	GSSAPI_STUB_CALL(inquire_mechs_for_name, minor_status, input_name, mech_types);
}

UINT32 SSPI_GSSAPI sspi_gss_sign(
	UINT32* minor_status,
	sspi_gss_ctx_id_t context_handle,
	int qop_req,
	sspi_gss_buffer_t message_buffer,
	sspi_gss_buffer_t message_token)
{
	GSSAPI_STUB_CALL(sign, minor_status, context_handle, qop_req, message_buffer, message_token);
}

UINT32 SSPI_GSSAPI sspi_gss_verify(
	UINT32* minor_status,
	sspi_gss_ctx_id_t context_handle,
	sspi_gss_buffer_t message_buffer,
	sspi_gss_buffer_t token_buffer,
	int* qop_state)
{
	GSSAPI_STUB_CALL(verify, minor_status, context_handle, message_buffer, token_buffer, qop_state);
}

UINT32 SSPI_GSSAPI sspi_gss_seal(
	UINT32* minor_status,
	sspi_gss_ctx_id_t context_handle,
	int conf_req_flag,
	int qop_req,
	sspi_gss_buffer_t input_message_buffer,
	int* conf_state,
	sspi_gss_buffer_t output_message_buffer)
{
	GSSAPI_STUB_CALL(seal, minor_status, context_handle, conf_req_flag, qop_req,
			input_message_buffer, conf_state, output_message_buffer);
}

UINT32 SSPI_GSSAPI sspi_gss_unseal(
	UINT32* minor_status,
	sspi_gss_ctx_id_t context_handle,
	sspi_gss_buffer_t input_message_buffer,
	sspi_gss_buffer_t output_message_buffer,
	int* conf_state,
	int* qop_state)
{
	GSSAPI_STUB_CALL(unseal, minor_status, context_handle, input_message_buffer, output_message_buffer, conf_state, qop_state);
}

UINT32 SSPI_GSSAPI sspi_gss_export_name(
	UINT32* minor_status,
	const sspi_gss_name_t input_name,
	sspi_gss_buffer_t exported_name)
{
	GSSAPI_STUB_CALL(export_name, minor_status, input_name, exported_name);
}

UINT32 SSPI_GSSAPI sspi_gss_duplicate_name(
	UINT32* minor_status,
	const sspi_gss_name_t input_name,
	sspi_gss_name_t* dest_name)
{
	GSSAPI_STUB_CALL(duplicate_name, minor_status, input_name, dest_name);
}

UINT32 SSPI_GSSAPI sspi_gss_canonicalize_name(
	UINT32* minor_status,
	const sspi_gss_name_t input_name,
	const sspi_gss_OID mech_type,
	sspi_gss_name_t* output_name)
{
	GSSAPI_STUB_CALL(canonicalize_name, minor_status, input_name, mech_type, output_name);
}

UINT32 SSPI_GSSAPI sspi_gss_pseudo_random(
	UINT32* minor_status,
	sspi_gss_ctx_id_t context,
	int prf_key,
	const sspi_gss_buffer_t prf_in,
	ssize_t desired_output_len,
	sspi_gss_buffer_t prf_out)
{
	GSSAPI_STUB_CALL(pseudo_random, minor_status, context, prf_key, prf_in, desired_output_len, prf_out);
}

UINT32 SSPI_GSSAPI sspi_gss_store_cred(
	UINT32* minor_status,
	const sspi_gss_cred_id_t input_cred_handle,
	sspi_gss_cred_usage_t input_usage,
	const sspi_gss_OID desired_mech,
	UINT32 overwrite_cred,
	UINT32 default_cred,
	sspi_gss_OID_set* elements_stored,
	sspi_gss_cred_usage_t* cred_usage_stored)
{
	GSSAPI_STUB_CALL(store_cred, minor_status, input_cred_handle, input_usage, desired_mech,
			overwrite_cred, default_cred, elements_stored, cred_usage_stored);
}

UINT32 SSPI_GSSAPI sspi_gss_set_neg_mechs(
	UINT32* minor_status,
	sspi_gss_cred_id_t cred_handle,
	const sspi_gss_OID_set mech_set)
{
	GSSAPI_STUB_CALL(set_neg_mechs, minor_status, cred_handle, mech_set);
}

/**
 * SSPI GSSAPI Helpers
 */

int sspi_GssSpnToServiceName(char* server, sspi_gss_name_t* name)
{
	UINT32 maj_stat;
	UINT32 min_stat;
	sspi_gss_buffer_desc input_msg_buffer;

	input_msg_buffer.value = server;
	input_msg_buffer.length = strlen(input_msg_buffer.value) + 1;

	maj_stat = sspi_gss_import_name(&min_stat, &input_msg_buffer, SSPI_GSS_C_NT_HOSTBASED_SERVICE, name);

	if (SSPI_GSS_ERROR(maj_stat))
	{
		WLog_ERR(TAG, "error: gss_import_name failed");
		return 0;
	}

	return 1;
}

int sspi_GssWrap(sspi_gss_ctx_id_t ctx, PSecBuffer in, PSecBuffer out)
{
	int state;
	UINT32 maj_stat;
	UINT32 min_stat;
	sspi_gss_buffer_desc input;
	sspi_gss_buffer_desc output;

	input.value = in->pvBuffer;
	input.length = in->cbBuffer;

	maj_stat = sspi_gss_wrap(&min_stat, ctx, TRUE, SSPI_GSS_C_QOP_DEFAULT, &input, &state, &output);

	if (SSPI_GSS_ERROR(maj_stat))
	{
		WLog_ERR(TAG, "error: gss_wrap failed");
		return 0;
	}

	if (state == 0)
	{
		WLog_ERR(TAG, "error: gss_wrap OK, but encryption and integrity are disabled");
		sspi_gss_release_buffer(&min_stat, &output);
		return 0;
	}

	CopyMemory(out->pvBuffer, output.value, output.length);
	sspi_gss_release_buffer(&min_stat, &output);

	return 1;
}

int sspi_GssUnwrap(sspi_gss_ctx_id_t ctx, PSecBuffer in, PSecBuffer out)
{
	int state;
	UINT32 maj_stat;
	UINT32 min_stat;
	sspi_gss_buffer_desc input;
	sspi_gss_buffer_desc output;

	input.value = in->pvBuffer;
	input.length = in->cbBuffer;

	maj_stat = sspi_gss_unwrap(&min_stat, ctx, &input, &output, &state, NULL);

	if (SSPI_GSS_ERROR(maj_stat))
	{
		WLog_ERR(TAG, "error: gss_unwrap failed");
		return 0;
	}

	if (state == 0)
	{
		WLog_ERR(TAG, "error: gss_unwrap OK, but encryption and integrity are disabled");
		sspi_gss_release_buffer(&min_stat, &output);
		return 0;
	}

	CopyMemory(out->pvBuffer, output.value, output.length);
	sspi_gss_release_buffer(&min_stat, &output);

	return 1;
}

#ifdef WITH_GSSAPI

#include <gssapi/gssapi.h>

GSSAPI_FUNCTION_TABLE g_GssApiLink =
{
	(fn_sspi_gss_acquire_cred) gss_acquire_cred, /* gss_acquire_cred */
	(fn_sspi_gss_release_cred) gss_release_cred, /* gss_release_cred */
	(fn_sspi_gss_init_sec_context) gss_init_sec_context, /* gss_init_sec_context */
	(fn_sspi_gss_accept_sec_context) gss_accept_sec_context, /* gss_accept_sec_context */
	(fn_sspi_gss_process_context_token) gss_process_context_token, /* gss_process_context_token */
	(fn_sspi_gss_delete_sec_context) gss_delete_sec_context, /* gss_delete_sec_context */
	(fn_sspi_gss_context_time) gss_context_time, /* gss_context_time */
	(fn_sspi_gss_get_mic) gss_get_mic, /* gss_get_mic */
	(fn_sspi_gss_verify_mic) gss_verify_mic, /* gss_verify_mic */
	(fn_sspi_gss_wrap) gss_wrap, /* gss_wrap */
	(fn_sspi_gss_unwrap) gss_unwrap, /* gss_unwrap */
	(fn_sspi_gss_display_status) gss_display_status, /* gss_display_status */
	(fn_sspi_gss_indicate_mechs) gss_indicate_mechs, /* gss_indicate_mechs */
	(fn_sspi_gss_compare_name) gss_compare_name, /* gss_compare_name */
	(fn_sspi_gss_display_name) gss_display_name, /* gss_display_name */
	(fn_sspi_gss_import_name) gss_import_name, /* gss_import_name */
	(fn_sspi_gss_release_name) gss_release_name, /* gss_release_name */
	(fn_sspi_gss_release_buffer) gss_release_buffer, /* gss_release_buffer */
	(fn_sspi_gss_release_oid_set) gss_release_oid_set, /* gss_release_oid_set */
	(fn_sspi_gss_inquire_cred) gss_inquire_cred, /* gss_inquire_cred */
	(fn_sspi_gss_inquire_context) gss_inquire_context, /* gss_inquire_context */
	(fn_sspi_gss_wrap_size_limit) gss_wrap_size_limit, /* gss_wrap_size_limit */
#if 0
	(fn_sspi_gss_import_name_object) gss_import_name_object, /* gss_import_name_object */
	(fn_sspi_gss_export_name_object) gss_export_name_object, /* gss_export_name_object */
#else
	(fn_sspi_gss_import_name_object) NULL, /* gss_import_name_object */
	(fn_sspi_gss_export_name_object) NULL, /* gss_export_name_object */
#endif
	(fn_sspi_gss_add_cred) gss_add_cred, /* gss_add_cred */
	(fn_sspi_gss_inquire_cred_by_mech) gss_inquire_cred_by_mech, /* gss_inquire_cred_by_mech */
	(fn_sspi_gss_export_sec_context) gss_export_sec_context, /* gss_export_sec_context */
	(fn_sspi_gss_import_sec_context) gss_import_sec_context, /* gss_import_sec_context */
	(fn_sspi_gss_release_oid) gss_release_oid, /* gss_release_oid */
	(fn_sspi_gss_create_empty_oid_set) gss_create_empty_oid_set, /* gss_create_empty_oid_set */
	(fn_sspi_gss_add_oid_set_member) gss_add_oid_set_member, /* gss_add_oid_set_member */
	(fn_sspi_gss_test_oid_set_member) gss_test_oid_set_member, /* gss_test_oid_set_member */
	(fn_sspi_gss_str_to_oid) gss_str_to_oid, /* gss_str_to_oid */
	(fn_sspi_gss_oid_to_str) gss_oid_to_str, /* gss_oid_to_str */
	(fn_sspi_gss_inquire_names_for_mech) gss_inquire_names_for_mech, /* gss_inquire_names_for_mech */
	(fn_sspi_gss_inquire_mechs_for_name) gss_inquire_mechs_for_name, /* gss_inquire_mechs_for_name */
	(fn_sspi_gss_sign) gss_sign, /* gss_sign */
	(fn_sspi_gss_verify) gss_verify, /* gss_verify */
	(fn_sspi_gss_seal) gss_seal, /* gss_seal */
	(fn_sspi_gss_unseal) gss_unseal, /* gss_unseal */
	(fn_sspi_gss_export_name) gss_export_name, /* gss_export_name */
	(fn_sspi_gss_duplicate_name) gss_duplicate_name, /* gss_duplicate_name */
	(fn_sspi_gss_canonicalize_name) gss_canonicalize_name, /* gss_canonicalize_name */
	(fn_sspi_gss_pseudo_random) gss_pseudo_random, /* gss_pseudo_random */
	(fn_sspi_gss_store_cred) gss_store_cred, /* gss_store_cred */
	(fn_sspi_gss_set_neg_mechs) gss_set_neg_mechs, /* gss_set_neg_mechs */
};

#endif

GSSAPI_FUNCTION_TABLE g_GssApi =
{
	NULL, /* gss_acquire_cred */
	NULL, /* gss_release_cred */
	NULL, /* gss_init_sec_context */
	NULL, /* gss_accept_sec_context */
	NULL, /* gss_process_context_token */
	NULL, /* gss_delete_sec_context */
	NULL, /* gss_context_time */
	NULL, /* gss_get_mic */
	NULL, /* gss_verify_mic */
	NULL, /* gss_wrap */
	NULL, /* gss_unwrap */
	NULL, /* gss_display_status */
	NULL, /* gss_indicate_mechs */
	NULL, /* gss_compare_name */
	NULL, /* gss_display_name */
	NULL, /* gss_import_name */
	NULL, /* gss_release_name */
	NULL, /* gss_release_buffer */
	NULL, /* gss_release_oid_set */
	NULL, /* gss_inquire_cred */
	NULL, /* gss_inquire_context */
	NULL, /* gss_wrap_size_limit */
	NULL, /* gss_import_name_object */
	NULL, /* gss_export_name_object */
	NULL, /* gss_add_cred */
	NULL, /* gss_inquire_cred_by_mech */
	NULL, /* gss_export_sec_context */
	NULL, /* gss_import_sec_context */
	NULL, /* gss_release_oid */
	NULL, /* gss_create_empty_oid_set */
	NULL, /* gss_add_oid_set_member */
	NULL, /* gss_test_oid_set_member */
	NULL, /* gss_str_to_oid */
	NULL, /* gss_oid_to_str */
	NULL, /* gss_inquire_names_for_mech */
	NULL, /* gss_inquire_mechs_for_name */
	NULL, /* gss_sign */
	NULL, /* gss_verify */
	NULL, /* gss_seal */
	NULL, /* gss_unseal */
	NULL, /* gss_export_name */
	NULL, /* gss_duplicate_name */
	NULL, /* gss_canonicalize_name */
	NULL, /* gss_pseudo_random */
	NULL, /* gss_store_cred */
	NULL, /* gss_set_neg_mechs */
};

#define SSPI_GSSAPI_LOAD_PROC(_fn) \
	g_GssApi.gss_ ## _fn = (fn_sspi_gss_ ##_fn) GetProcAddress(g_GssApiModule, "gss_" #_fn)

int sspi_GssApiInit()
{
	if (g_Initialized)
		return 1;

	g_Initialized = TRUE;

#ifdef WITH_GSSAPI
	CopyMemory(&g_GssApi, &g_GssApiLink, sizeof(GSSAPI_FUNCTION_TABLE));
	return 1;
#endif

	g_GssApiModule = LoadLibraryA("libgssapi_krb5.so");

	if (!g_GssApiModule)
		return -1;

	SSPI_GSSAPI_LOAD_PROC(acquire_cred);
	SSPI_GSSAPI_LOAD_PROC(release_cred);
	SSPI_GSSAPI_LOAD_PROC(init_sec_context);
	SSPI_GSSAPI_LOAD_PROC(accept_sec_context);
	SSPI_GSSAPI_LOAD_PROC(process_context_token);
	SSPI_GSSAPI_LOAD_PROC(delete_sec_context);
	SSPI_GSSAPI_LOAD_PROC(context_time);
	SSPI_GSSAPI_LOAD_PROC(get_mic);
	SSPI_GSSAPI_LOAD_PROC(verify_mic);
	SSPI_GSSAPI_LOAD_PROC(wrap);
	SSPI_GSSAPI_LOAD_PROC(unwrap);
	SSPI_GSSAPI_LOAD_PROC(display_status);
	SSPI_GSSAPI_LOAD_PROC(indicate_mechs);
	SSPI_GSSAPI_LOAD_PROC(compare_name);
	SSPI_GSSAPI_LOAD_PROC(display_name);
	SSPI_GSSAPI_LOAD_PROC(import_name);
	SSPI_GSSAPI_LOAD_PROC(release_name);
	SSPI_GSSAPI_LOAD_PROC(release_buffer);
	SSPI_GSSAPI_LOAD_PROC(release_oid_set);
	SSPI_GSSAPI_LOAD_PROC(inquire_cred);
	SSPI_GSSAPI_LOAD_PROC(inquire_context);
	SSPI_GSSAPI_LOAD_PROC(wrap_size_limit);
	SSPI_GSSAPI_LOAD_PROC(import_name_object);
	SSPI_GSSAPI_LOAD_PROC(export_name_object);
	SSPI_GSSAPI_LOAD_PROC(add_cred);
	SSPI_GSSAPI_LOAD_PROC(inquire_cred_by_mech);
	SSPI_GSSAPI_LOAD_PROC(export_sec_context);
	SSPI_GSSAPI_LOAD_PROC(import_sec_context);
	SSPI_GSSAPI_LOAD_PROC(release_oid);
	SSPI_GSSAPI_LOAD_PROC(create_empty_oid_set);
	SSPI_GSSAPI_LOAD_PROC(add_oid_set_member);
	SSPI_GSSAPI_LOAD_PROC(test_oid_set_member);
	SSPI_GSSAPI_LOAD_PROC(str_to_oid);
	SSPI_GSSAPI_LOAD_PROC(oid_to_str);
	SSPI_GSSAPI_LOAD_PROC(inquire_names_for_mech);
	SSPI_GSSAPI_LOAD_PROC(inquire_mechs_for_name);
	SSPI_GSSAPI_LOAD_PROC(sign);
	SSPI_GSSAPI_LOAD_PROC(verify);
	SSPI_GSSAPI_LOAD_PROC(seal);
	SSPI_GSSAPI_LOAD_PROC(unseal);
	SSPI_GSSAPI_LOAD_PROC(export_name);
	SSPI_GSSAPI_LOAD_PROC(duplicate_name);
	SSPI_GSSAPI_LOAD_PROC(canonicalize_name);
	SSPI_GSSAPI_LOAD_PROC(pseudo_random);
	SSPI_GSSAPI_LOAD_PROC(store_cred);
	SSPI_GSSAPI_LOAD_PROC(set_neg_mechs);

	return 1;
}
