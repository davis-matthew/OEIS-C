/*
 * A013818  a(n) = 23^(4*n + 1).
 * https://oeis.org/A013818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23^(4*n+1) Recurrence: a(0) = 23, a(n) = 279841 * a(n-1)
 *            for n >= 1
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of initialized entries */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* Ensure cache is initialized */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) {
            /* Out of memory: return 0 as a graceful fallback */
            return 0;
        }
        cache[0] = (seq_t)23;  /* a(0) */
        cache_len = 1;
    }

    /* Grow capacity if needed */
    if (n + 1 > cache_cap) {
        /* Simple growth strategy: double until large enough */
        while (n + 1 > cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 2;
        }
        seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory: return last cached value */
            return cache[cache_len - 1];
        }
        cache = new_cache;
    }

    /* Compute new entries up to n */
    while (cache_len <= n) {
        /* Multiply previous term by 23^4 = 279841 */
        cache[cache_len] = cache[cache_len - 1] * (seq_t)279841;
        cache_len++;
    }

    return cache[n];
}
