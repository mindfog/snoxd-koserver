#pragma once

#include "OdbcParameter.h"
#include <map>

class OdbcConnection;
class OdbcCommand
{
public:
	typedef std::map<int, OdbcParameter *> OdbcParameterCollection;

	OdbcCommand(HDBC conn);
	OdbcCommand(OdbcConnection * conn);

	__forceinline bool isOpen() { return m_hStmt != NULL; };
	__forceinline TCHAR * GetError() { return (TCHAR *)m_szError.c_str();  };
	__forceinline bool hasData() { return m_resultCode != SQL_NO_DATA; };

	bool Execute(const tstring & szSQL);
	bool MoveNext();
	bool MoveNextSet();

	bool Prepare(const tstring & szSQL);

#define ADD_ODBC_PARAMETER(name, type, sqlType) void AddParameter(SQLSMALLINT paramType, type *value, SQLLEN maxLength = sizeof(type)); \
	type OdbcCommand::Fetch ## name(int pos, SQLLEN maxLength = 0);

ADD_ODBC_PARAMETER(Byte, uint8, SQL_C_TINYINT)
ADD_ODBC_PARAMETER(SByte, int8, SQL_C_STINYINT)
ADD_ODBC_PARAMETER(Char, char, SQL_C_CHAR)
ADD_ODBC_PARAMETER(UInt16, uint16, SQL_C_USHORT)
ADD_ODBC_PARAMETER(Int16, int16, SQL_C_SSHORT)
ADD_ODBC_PARAMETER(UInt32, uint32, SQL_C_ULONG)
ADD_ODBC_PARAMETER(Int32, int32, SQL_C_LONG)
ADD_ODBC_PARAMETER(Single, float, SQL_C_FLOAT)
ADD_ODBC_PARAMETER(Double, double, SQL_C_DOUBLE)
tstring OdbcCommand::FetchString(int pos, SQLLEN maxLength = 0);
void OdbcCommand::FetchString(int pos, TCHAR * charArray, SQLLEN maxLength);
#undef ADD_ODBC_PARAMETER

	void Detach();
	~OdbcCommand();

private:
	bool Open();
	void ClearParameters();
public:	void Close();
private:
	HDBC m_connHandle;
	OdbcConnection *m_odbcConnection;
	HSTMT m_hStmt;

	OdbcParameterCollection m_params;

	tstring m_szError;
	SQLRETURN m_resultCode;
};