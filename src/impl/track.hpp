/**
 * Copyright (c) 2020-2021 Paul-Louis Ageneau
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef RTC_IMPL_TRACK_H
#define RTC_IMPL_TRACK_H

#include "channel.hpp"
#include "description.hpp"
#include "common.hpp"
#include "mediahandler.hpp"

#if RTC_ENABLE_MEDIA
#include "dtlssrtptransport.hpp"
#endif

#include <atomic>
#include <shared_mutex>
#include <variant>

namespace rtc::impl {

class Track final : public std::enable_shared_from_this<Track>, public Channel {
public:
	Track(Description::Media description);
	~Track() = default;

	void close();
	void incoming(message_ptr message);
	bool outgoing(message_ptr message);

	std::optional<message_variant> receive() override;
	std::optional<message_variant> peek() override;
	size_t availableAmount() const override;

	bool isOpen() const;
	bool isClosed() const;

	string mid() const;
	Description::Direction direction() const;
	Description::Media description() const;
	void setDescription(Description::Media description);

	std::shared_ptr<MediaHandler> getRtcpHandler();
	void setRtcpHandler(shared_ptr<MediaHandler> handler);

#if RTC_ENABLE_MEDIA
	void open(std::shared_ptr<DtlsSrtpTransport> transport);
#endif

private:
#if RTC_ENABLE_MEDIA
	weak_ptr<DtlsSrtpTransport> mDtlsSrtpTransport;
#endif

	Description::Media mMediaDescription;
	shared_ptr<MediaHandler> mRtcpHandler;

	mutable std::shared_mutex mMutex;

	std::atomic<bool> mIsClosed = false;

	Queue<message_ptr> mRecvQueue;
};

} // namespace rtc::impl

#endif
