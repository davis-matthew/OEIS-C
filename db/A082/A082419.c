/*
 * A082419  Numbers n such that P(n+2) < P(n) < P(n+1), where P(n) = largest
 *          prime factor of n (A006530).
 * https://oeis.org/A082419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A082419 by a simple linear search with caching.
   Each term n is the smallest integer m such that
   P(m+2) < P(m) < P(m+1), where P(k) = largest prime factor of k.
   The function uses the provided utility
   uint64_t largest_prime_factor(uint64_t n). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/largest_prime_factor.h"   /* Declaration of the helper */

/* External sequence type definitions are supplied by the build. */
extern uint64_t largest_prime_factor(uint64_t n);

seq_t a(idx_t n)
{
    /* Static cache of the computed sequence values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* Number of valid entries in cache. */
    static size_t cache_cap = 0;   /* Allocated capacity of cache. */

    /* The next integer to test for the property. */
    static uint64_t next_candidate = 2;

    /* Ensure the cache holds at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Search for the next matching integer. */
        for (uint64_t m = next_candidate; ; ++m)
        {
            uint64_t p_m  = largest_prime_factor(m);
            uint64_t p_m1 = largest_prime_factor(m + 1);
            uint64_t p_m2 = largest_prime_factor(m + 2);

            if (p_m2 < p_m && p_m < p_m1)
            {
                /* Found a term.  Grow the cache if necessary. */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();   /* Out of memory. */
                    cache = tmp;
                    cache_cap = new_cap;
                }

                cache[cache_len++] = (seq_t)m;

                /* Next search starts after this m. */
                next_candidate = m + 1;
                break;  /* Exit inner search loop. */
            }
        }
    }

    return cache[(size_t)n - 1];
}
