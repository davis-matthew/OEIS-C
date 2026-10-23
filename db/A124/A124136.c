/*
 * A124136  The list of primes p such that the number of primes in the open
 *          interval (p,2*p) is larger than the number of primes in the open
 *          interval (q,2*q) for all q<p.
 * https://oeis.org/A124136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A124136: primes p such that the number of primes in the open interval
   (p,2p) is greater than for all smaller primes.  We compute
   π(2p-1) – π(p) using the utility compute_pi() and keep a running
   record of the maximum value seen.  The function returns the nth prime
   that sets a new record. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_pi.h"   /* provides: seq_t compute_pi(idx_t n) */

seq_t a(idx_t n)
{
    /* Static state preserved across calls. */
    static bool            init   = false;
    static uint64_t *      primes = NULL;      /* found primes (uint64_t)     */
    static size_t          plen   = 0;          /* number of primes stored     */
    static size_t          pcap   = 0;          /* capacity of primes array    */
    static uint64_t        next   = 3;          /* next odd candidate          */

    static seq_t *        records = NULL;       /* record primes (seq_t)       */
    static size_t         rlen   = 0;          /* number of record primes     */
    static size_t         rcap   = 0;          /* capacity of records array  */
    static seq_t          rmax   = 0;          /* maximum count so far        */

    if (!init) {   /* one‑time allocation and initialisation */
        pcap   = 16;
        primes = (uint64_t *)malloc(pcap * sizeof(uint64_t));
        if (!primes) exit(1);
        primes[0] = 2;
        plen = 1;

        rcap   = 16;
        records = (seq_t *)malloc(rcap * sizeof(seq_t));
        if (!records) exit(1);
        records[0] = (seq_t)2;
        rlen = 1;
        rmax = 1;

        init = true;
    }

    /* Extend the record list until we have at least n entries. */
    while (rlen < (size_t)n) {
        uint64_t p = next;
        next += 2;   /* consider only odd candidates after 3 */

        /* Trial division by known primes. */
        bool prime = true;
        for (size_t i = 0; i < plen && primes[i] * primes[i] <= p; ++i) {
            if (p % primes[i] == 0) {
                prime = false;
                break;
            }
        }
        if (!prime)
            continue;   /* composite – skip */

        /* Store the new prime. */
        if (plen == pcap) {
            pcap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(primes, pcap * sizeof(uint64_t));
            if (!tmp) exit(1);
            primes = tmp;
        }
        primes[plen++] = p;

        /* Count primes in the open interval (p,2p). */
        seq_t cnt = compute_pi((idx_t)((p << 1) - 1)) - compute_pi((idx_t)p);

        /* New record? */
        if (cnt > rmax) {
            rmax = cnt;
            if (rlen == rcap) {
                rcap <<= 1;
                seq_t *tmp = (seq_t *)realloc(records, rcap * sizeof(seq_t));
                if (!tmp) exit(1);
                records = tmp;
            }
            records[rlen++] = (seq_t)p;
        }
    }

    return records[n - 1];
}
