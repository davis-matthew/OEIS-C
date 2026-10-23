/*
 * A013889  a(n) = 18^(5*n + 4).
 * https://oeis.org/A013889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18^(5*n + 4) Recurrence: a(0) = 104976, a(n) = 1889568 *
 *            a(n–1) (n ≥ 1)
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Base term a(0) and the fixed multiplier 18^5 = 1889568 */
    static const seq_t base  = (seq_t)104976;
    static const seq_t mult  = (seq_t)1889568;

    /* Cache for previously computed values */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* Number of entries cached (≥1 after init) */
    static size_t cache_cap = 0;   /* Allocated capacity of cache */

    /* Initialise cache with a(0) on first call */
    if (!cache) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return base; /* fallback – unlikely to happen */
        cache[0] = base;
        cache_len = 1;
    }

    /* Grow the cache if we have not yet computed up to n */
    if ((size_t)n >= cache_len) {
        /* Allocate a larger buffer if necessary */
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cache_cap) {
            cache_cap = new_cap;
            seq_t *new_cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            if (!new_cache) return base; /* fallback */
            cache = new_cache;
        }
        /* Compute missing terms */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = cache[i - 1] * mult;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
