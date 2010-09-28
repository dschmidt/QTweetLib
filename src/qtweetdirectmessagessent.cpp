/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include <QNetworkRequest>
#include <QNetworkReply>
#include "qtweetdirectmessagessent.h"
#include "qtweetdmstatus.h"

QTweetDirectMessagesSent::QTweetDirectMessagesSent(QObject *parent) :
        QTweetNetBase(parent)
{
}

QTweetDirectMessagesSent::QTweetDirectMessagesSent(OAuthTwitter *oauthTwitter, QObject *parent) :
        QTweetNetBase(oauthTwitter, parent)
{
}

/*!
    \param sinceid Returns results with an ID greater than (that is, more recent than) the specified ID.
    \param maxid Returns results with an ID less than (that is, older than) or equal to the specified ID.
    \param count Specifies the number of records to retrieve. Must be less than or equal to 200.
    \param page Specifies the page of results to retrieve.
    \param includeEntities When set to true, each tweet will include a node called "entities,".
    \param respType Response type json or xml
 */
void QTweetDirectMessagesSent::fetch(qint64 sinceid,
                                     qint64 maxid,
                                     int count,
                                     int page,
                                     bool includeEntities,
                                     ResponseType respType)
{
    Q_ASSERT(oauthTwitter() != 0);

    QUrl url;

    if (respType == QTweetNetBase::JSON)
        url.setUrl("http://api.twitter.com/1/direct_messages/sent.json");
    else
        url.setUrl("http://api.twitter.com/1/direct_messages/sent.xml");

    if (sinceid)
        url.addQueryItem("since_id", QString::number(sinceid));

    if (maxid)
        url.addQueryItem("max_id", QString::number(maxid));

    if (count)
        url.addQueryItem("count", QString::number(count));

    if (page)
        url.addQueryItem("page", QString::number(page));

    if (includeEntities)
        url.addQueryItem("include_entities", "true");

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader(AUTH_HEADER, oauthHeader);

    QNetworkReply *reply = oauthTwitter()->networkAccessManager()->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(reply()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTweetDirectMessagesSent::parsingJsonFinished(const QVariant &json, bool ok, const QString &errorMsg)
{
    if (ok) {
        QList<QTweetDMStatus> directMessages = variantToDirectMessagesList(json);

        emit parsedDirectMessages(directMessages);
    } else {
        qDebug() << "QTweetDirectMessagesSent parser error: " << errorMsg;
    }
}

void QTweetDirectMessagesSent::error()
{
    // ### TODO
}
