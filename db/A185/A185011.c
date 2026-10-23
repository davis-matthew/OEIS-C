/*
 * A185011  Numbers k such that P(k^2+1) < P((k+1)^2+1) where P(n) (A006530)
 *          is the largest prime factor of n.
 * https://oeis.org/A185011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 131, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Algorithm: iterate k from 1 upwards, keep those k for which the
 *  largest prime factor of k^2+1 is strictly smaller than that of
 *  (k+1)^2+1.  The seq_t array 'cache' stores the sequence values
 *  a(1), a(2), … in ascending order.  Subsequent calls extend the
 *  cache only as far as needed.  The routine uses the helper
 *  largest_prime_factor from the utils library.  All arithmetic is
 *  performed with 64‑bit integers; k^2+1 is guaranteed to fit into
 *  uint64_t for the domain of n considered (n ≤ 10000).  The code
 *  is fully static‑storage and requires no external state. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    /* static cache of previously computed sequence values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;        /* number of elements stored */
    static size_t cache_cap = 0;        /* allocated capacity */
    static uint64_t last_k = 0;         /* largest k that has been tested */

    /* Validate input */
    if (n == 0) {
        /* undefined, return 0 as a safe default */
        return (seq_t)0;
    }

    /* Ensure we have computed at least n elements */
    while (cache_len < n) {
        uint64_t k = ++last_k;                         /* candidate k */
        uint64_t k2_plus = k * k + 1ULL;              /* k^2 + 1   */
        uint64_t kp1 = k + 1U;
        uint64_t kp1_2_plus = kp1 * kp1 + 1ULL;        /* (k+1)^2 +1*/

        uint64_t p_k   = largest_prime_factor(k2_plus);
        uint64_t p_kp1 = largest_prime_factor(kp1_2_plus);

        if (p_k < p_kp1) {
            /* extend the cache if necessary */
            if (cache_cap == 0) {
                cache_cap = 128;
                cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            } else if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
