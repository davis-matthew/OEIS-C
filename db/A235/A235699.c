/*
 * A235699  a(n+1) = a(n) + (a(n) mod 10) + 1, a(0) = 0.
 * https://oeis.org/A235699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n+1) = a(n) + (a(n) mod 10) + 1, a(0) = 0
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Ensure we have at least the first term. */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = seq_alloc(cache_cap);
        if (!cache) abort();
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Grow the cache if more terms are requested. */
    if (n + 1 > cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = seq_realloc(cache, cache_cap, cache_len);
        if (!cache) abort();
        cache_cap = cache_len;

        for (idx_t i = old_len - 1; i < n; ++i) {
            cache[i + 1] = cache[i] + (cache[i] % (seq_t)10) + (seq_t)1;
        }
    }

    return cache[n];
}
