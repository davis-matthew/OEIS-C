/*
 * A189748  a(n) = 5*a(n-1) + 4*a(n-2) with a(1)=5, a(2)=4.
 * https://oeis.org/A189748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 4*a(n-2) * Initial values: a(1) = 5, a(2) = 4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    /* Ensure cache can hold up to index n-1 (0‑based) */
    if (n > cache_len) {
        /* Expand capacity if needed */
        if (cache_cap == 0) cache_cap = 8;
        while (cache_cap < n) cache_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure: abort */
            abort();
        }
        cache = tmp;
        /* Initialize new slots if expansion allowed */
        /* Compute missing terms iteratively */
        if (cache_len == 0) {
            /* No terms known: set first two */
            cache[0] = 5;            /* a(1) */
            if (n > 1) cache[1] = 4;  /* a(2) */
            cache_len = (n > 1) ? 2 : 1;
        } else {
            /* Compute from cache_len to n */
            /* Ensure we have at least two base cases */
            if (cache_len == 1) {
                cache[1] = 4;
                cache_len = 2;
            }
            for (size_t i = cache_len; i < n; ++i) {
                cache[i] = 5 * cache[i-1] + 4 * cache[i-2];
            }
            cache_len = n;
        }
    } else {
        /* If cache is already long enough, ensure base cases exist */
        if (cache_len < 2) {
            if (cache_len == 0) cache[0] = 5;
            if (cache_len == 1) cache[1] = 4;
            cache_len = cache_cap > 2 ? 2 : cache_len;
        }
    }
    /* Return requested term (1‑based). */
    return cache[n-1];
}
