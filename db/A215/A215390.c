/*
 * A215390  Primes congruent to {1, 2} mod 11.
 * https://oeis.org/A215390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iteratively search for primes ≡1 or 2 (mod 11),
   caching found values for repeated calls.  This provides any
   requested term a(n) in time proportional to the index (≈O(n)). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Cache of already found terms */
    static seq_t *cache      = NULL;
    static size_t cache_len   = 0;
    static size_t cache_cap   = 0;
    /* Next integer to test; starts at 2 */
    static uint64_t next_candidate = 2;

    /* Keep building the cache until it holds at least n terms */
    while (cache_len < n)
    {
        /* Test current candidate for primality */
        if (is_prime((idx_t)next_candidate))
        {
            uint64_t r = next_candidate % 11ULL;
            if (r == 1ULL || r == 2ULL)
            {
                /* Grow cache if needed */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_candidate;
            }
        }
        next_candidate++;      /* Move to next integer */
    }

    return cache[n - 1];      /* 1‑based indexing */
}
