/*
 * A161824  Numbers such that A010060(n) = A010060(n+6).
 * https://oeis.org/A161824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer m (m ≥ 0) such that parity(popcount(m))
 *            * equals parity(popcount(m+6)). Indices are 1‑based; a(1)=0
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>            /* for size_t */
#include <stdbool.h>           /* for bool */
#include "../utils/ensure_capacity.h"   /* ensure_capacity() */

/* Parity of number of set bits in an idx_t value. */
static inline bool parity_uint64(idx_t x)
{
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (x & 1) != 0;
}

/* Returns a(n) for the OEIS sequence A161824. */
seq_t a(idx_t n)
{
    /* Static cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_candidate = 0; /* next integer to test */

    /* Guard against n == 0 (offset specifies a(1) is first term). */
    if (n == 0) {
        return 0;
    }

    /* Ensure we have at least n terms in the cache. */
    while (cache_len < n) {
        /* Check if current candidate satisfies parity condition. */
        if (parity_uint64(next_candidate) == parity_uint64(next_candidate + 6)) {
            /* Grow cache if necessary. */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                if (new_cap < cache_len + 1) {
                    new_cap = cache_len + 1;
                }
                ensure_capacity(new_cap, &cache_cap, (void **)&cache,
                                sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    /* Return the requested 1‑based term. */
    return cache[n - 1];
}
