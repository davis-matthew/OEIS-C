/*
 * A063173  Prime-composite array T(m,n): highest power of the n-th prime that
 *          divides the m-th composite, read by antidiagonals.
 * https://oeis.org/A063173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   ensure_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/******************************************************************************
 * Formula: A063173
 * a(n) = highest exponent of the n‑th prime that divides the m‑th composite,
 * where (m,k) is obtained by reading the table T(m,k) along antidiagonals.
 *
 * The mapping from a serial index n (n≥1) to the pair (m,k) is:
 *   let d be the smallest integer with d*(d+1)/2 >= n
 *   let s = d + 1
 *   let cum = (d-1)*d/2          // number of terms before diagonal d
 *   let pos = n - cum            // 1 ≤ pos ≤ s-1
 *   then m = s - pos             // composite index (1‑based)
 *        k = pos                 // prime index    (1‑based)
 *
 * The function keeps an expanding list of primes and composites and returns
 * the exponent of the k‑th prime in the m‑th composite.
 ******************************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ensure_cap.h"

/* storage for primes and composites */
static uint64_t *primes = NULL;
static size_t primes_cap = 0, primes_cnt = 0;
static uint64_t *comps  = NULL;
static size_t comps_cap = 0, comps_cnt = 0;
static uint64_t next_num = 4;   /* next integer to test */

/* preliminary seeding of the prime list with 2,3 */
static void seed_primes(void) {
    if (primes_cnt == 0) {
        /* ensure capacity for two elements */
        ensure_cap(&primes, &primes_cap, 2);
        primes[0] = 2;
        primes[1] = 3;
        primes_cnt = 2;
    }
}

/* generate numbers until we have at least m_needed composites
 * and at least p_needed primes                                */
static void generate_up_to(size_t m_needed, size_t p_needed) {
    seed_primes(); /* guarantee 2 and 3 are present */

    while (comps_cnt < m_needed || primes_cnt < p_needed) {
        uint64_t cur = next_num++;
        int is_prime = 1;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > cur / p) break;   /* p*p > cur   */
            if (cur % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            ensure_cap(&primes, &primes_cap, primes_cnt + 1);
            primes[primes_cnt++] = cur;
        } else {
            ensure_cap(&comps, &comps_cap, comps_cnt + 1);
            comps[comps_cnt++] = cur;
        }
    }
}

seq_t a(idx_t n) {
    /* 1) locate diagonal */
    idx_t s = 2;
    idx_t cum = 0;
    while (cum + (s - 1) < n) {      /* cum = (s-2)(s-1)/2 after loop */
        cum += s - 1;
        ++s;
    }
    idx_t pos   = n - cum;          /* 1 .. s-1 */
    idx_t m_idx = s - pos;          /* composite index (1‑based) */
    idx_t p_idx = pos;              /* prime index     */

    /* 2) ensure we have enough primes & composites */
    generate_up_to((size_t)m_idx, (size_t)p_idx);

    /* 3) compute exponent of prime in composite */
    uint64_t composite = comps[m_idx - 1];
    uint64_t prime     = primes[p_idx - 1];
    seq_t exp = 0;
    while (composite % prime == 0) {
        composite /= prime;
        exp += 1;
    }
    return exp;
}
