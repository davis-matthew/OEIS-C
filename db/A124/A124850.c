/*
 * A124850  Primes p=n/2 such that 7n+3 and 3n+7 are primes.
 * https://oeis.org/A124850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 168, u32 n <= 65, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124850:
   a(n) = the prime p such that 14p+3 and 6p+7 are both prime.
   The index a(1) corresponds to the first such prime.  */

#include <stdbool.h>
#include <stddef.h>

#include "../utils/ensure_capacity.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache for the sequence values */
    static seq_t *cache   = NULL;
    static size_t cache_cnt = 0;
    static size_t cache_cap = 0;

    /* static prime candidate generator */
    static seq_t next_cand = 2;
    static bool ready = false;

    if (!ready) {
        ready = true;
    }

    /* generate terms until we have at least n entries */
    while (cache_cnt < (size_t)n) {
        seq_t p = next_cand;

        /* advance candidate: after 2 go to 3, then only odds */
        if (p == (seq_t)2) {
            next_cand = (seq_t)3;
        } else {
            next_cand = p + (seq_t)2;
        }

        if (!is_prime_big(p)) {
            continue; /* skip composites */
        }

        /* compute 14p+3 and 6p+7 */
        seq_t v1 = (seq_t)14 * p + (seq_t)3;
        seq_t v2 = (seq_t)6  * p + (seq_t)7;

        if (is_prime_big(v1) && is_prime_big(v2)) {
            /* enlarge cache if required */
            if (cache_cnt + 1 > cache_cap) {
                ensure_capacity(cache_cnt + 1, &cache_cap,
                                (void **)&cache,
                                sizeof(seq_t));
            }
            cache[cache_cnt++] = p;
        }
    }

    return cache[(size_t)n - 1];
}
