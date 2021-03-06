/*
 * Copyright (c) 2019 Cisco and/or its affiliates.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __VOM_QOS_MARK_H__
#define __VOM_QOS_MARK_H__

#include <ostream>

#include "vom/interface.hpp"
#include "vom/qos_map.hpp"
#include "vom/qos_types.hpp"
#include "vom/singular_db.hpp"

namespace VOM {
namespace QoS {

/**
 * The marking of packets with QoS bits as they egress an interface
 */
class mark : public object_base
{
public:
  mark(const interface& i, const map& m, const source_t& source);
  mark(const mark& r);

  ~mark();

  typedef std::pair<interface::key_t, source_t> key_t;

  /**
   * Return the object's key
   */
  const key_t key() const;

  /**
   * comparison operator
   */
  bool operator==(const mark& bdae) const;

  /**
   * Return the matching 'singular instance'
   */
  std::shared_ptr<mark> singular() const;

  /**
   * Find the instnace of the bridge_domain domain in the OM
   */
  static std::shared_ptr<mark> find(const key_t& k);

  /**
   * Dump all bridge_domain-doamin into the stream provided
   */
  static void dump(std::ostream& os);

  /**
   * replay the object to create it in hardware
   */
  void replay(void);

  /**
   * Convert to string for debugging
   */
  std::string to_string() const;

private:
  /**
   * Class definition for listeners to OM events
   */
  class event_handler : public OM::listener, public inspect::command_handler
  {
  public:
    event_handler();
    virtual ~event_handler() = default;

    /**
     * Handle a populate event
     */
    void handle_populate(const client_db::key_t& key);

    /**
     * Handle a replay event
     */
    void handle_replay();

    /**
     * Show the object in the Singular DB
     */
    void show(std::ostream& os);

    /**
     * Get the sortable Id of the listener
     */
    dependency_t order() const;
  };

  /**
   * event_handler to register with OM
   */
  static event_handler m_evh;

  /**
   * Commit the acculmulated changes into VPP. i.e. to a 'HW" write.
   */
  void update(const mark& obj);

  /**
   * Find or add the instnace of the bridge_domain domain in the OM
   */
  static std::shared_ptr<mark> find_or_add(const mark& temp);

  /*
   * It's the VPPHW class that updates the objects in HW
   */
  friend class VOM::OM;

  /**
   * It's the singular_db class that calls replay()
   */
  friend class singular_db<key_t, mark>;

  /**
   * Sweep/reap the object if still stale
   */
  void sweep(void);

  /**
   * HW configuration for the config. The bool representing the
   * do/don't configured/unconfigured.
   */
  HW::item<bool> m_config;

  /**
   * The interface the mark applies to
   */
  std::shared_ptr<interface> m_itf;

  /**
   * The map the marking uses
   */
  std::shared_ptr<map> m_map;

  /**
   * QoS source to mark from
   */
  source_t m_src;

  /**
   * A map of all QoS Markers
   */
  static singular_db<key_t, mark> m_db;
};

}; // namesapce QoS

std::ostream& operator<<(std::ostream& os, const QoS::mark::key_t& key);

}; // namespace VOM

/*
 * fd.io coding-style-patch-verification: OFF
 *
 * Local Variables:
 * eval: (c-set-style "mozilla")
 * End:
 */

#endif
