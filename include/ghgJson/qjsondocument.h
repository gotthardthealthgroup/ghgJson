/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef GHG_QJSONDOCUMENT_H
#define GHG_QJSONDOCUMENT_H

#include "qjsonvalue.h"
#include "ghgjson_export.h"


class QDebug;

namespace ghg
{

namespace QJsonPrivate {
    class Parser;
}

struct GHGJSON_EXPORT QJsonParseError
{
    enum ParseError {
        NoError = 0,
        UnterminatedObject,
        MissingNameSeparator,
        UnterminatedArray,
        MissingValueSeparator,
        IllegalValue,
        TerminationByNumber,
        IllegalNumber,
        IllegalEscapeSequence,
        IllegalUTF8String,
        UnterminatedString,
        MissingObject,
        DeepNesting,
        DocumentTooLarge,
        GarbageAtEnd
    };

    QString    errorString() const;

    int        offset;
    ParseError error;
};

class GHGJSON_EXPORT QJsonDocument
{
public:
#ifdef Q_LITTLE_ENDIAN
    static const uint BinaryFormatTag = ('q') | ('b' << 8) | ('j' << 16) | ('s' << 24);
#else
    static const uint BinaryFormatTag = ('q' << 24) | ('b' << 16) | ('j' << 8) | ('s');
#endif

    QJsonDocument();
    explicit QJsonDocument(const QJsonObject &object);
    explicit QJsonDocument(const QJsonArray &array);
    ~QJsonDocument();

    QJsonDocument(const QJsonDocument &other);
    QJsonDocument &operator =(const QJsonDocument &other);

    enum DataValidation {
        Validate,
        BypassValidation
    };

    static QJsonDocument fromRawData(const char *data, int size, DataValidation validation = Validate);
    const char *rawData(int *size) const;

    static QJsonDocument fromBinaryData(const QByteArray &data, DataValidation validation  = Validate);
    QByteArray toBinaryData() const;

    static QJsonDocument fromVariant(const QVariant &variant);
    QVariant toVariant() const;

    enum JsonFormat {
        Indented,
        Compact
    };

    static QJsonDocument fromJson(const QByteArray &json, QJsonParseError *error = Q_NULLPTR);

#ifdef Q_QDOC
    QByteArray toJson(JsonFormat format = Indented) const;
#elif !defined(QT_JSON_READONLY)
    QByteArray toJson() const; //### Merge in Qt6
    QByteArray toJson(JsonFormat format) const;
#endif

    bool isEmpty() const;
    bool isArray() const;
    bool isObject() const;

    QJsonObject object() const;
    QJsonArray array() const;

    void setObject(const QJsonObject &object);
    void setArray(const QJsonArray &array);

    bool operator==(const QJsonDocument &other) const;
    bool operator!=(const QJsonDocument &other) const { return !(*this == other); }

    bool isNull() const;

private:
    friend class QJsonValue;
    friend class QJsonPrivate::Data;
    friend class QJsonPrivate::Parser;
    friend GHGJSON_EXPORT QDebug operator<<(QDebug, const QJsonDocument &);

    QJsonDocument(QJsonPrivate::Data *data);

    QJsonPrivate::Data *d;
};

#if !defined(QT_NO_DEBUG_STREAM) && !defined(QT_JSON_READONLY)
GHGJSON_EXPORT QDebug operator<<(QDebug, const QJsonDocument &);
#endif

}

#endif // GHG_QJSONDOCUMENT_H
