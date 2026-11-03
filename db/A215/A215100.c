/*
 * A215100  a(n) = 3*a(n-1) + 4*a(n-2) + a(n-3) with a(0)=2, a(1)=5, a(2)=22.
 * https://oeis.org/A215100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) + a(n-3) ; a(0)=2, a(1)=5, a(2)=22
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 738 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* memoization cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* handle small indices immediately */
    if (n < 3) {
        switch (n) {
            case 0: return (seq_t)2;
            case 1: return (seq_t)5;
            case 2: return (seq_t)22;
        }
    }

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_len = 3;
        cache = mem_new(seq_t, cache_len);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)22;
    }

    /* grow the cache if needed and compute new terms */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = mem_new(seq_t, new_len);

        /* copy existing values */
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }

        mem_free(cache);
        cache = new_cache;
        cache_len = new_len;

        /* compute forward from 3 to n */
        for (size_t i = 3; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)3 * cache[i-1]
                     + (seq_t)4 * cache[i-2]
                     +      cache[i-3];
        }
    }

    return cache[(size_t)n];
}
