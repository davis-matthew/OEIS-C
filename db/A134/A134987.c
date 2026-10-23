/*
 * A134987  Third extended Jacobsthal recurrence:
 *          a(n)=4a(n-1)-6(n-2)+4a(n-3)-a(n-4)+2a(n-5).
 * https://oeis.org/A134987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)-6*a(n-2)+4*a(n-3)-a(n-4)+2*a(n-5) with
 *            a(0)=a(1)=a(2)=a(3)=0, a(4)=1
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;  /* number of elements stored  (highest index + 1) */
    static size_t cache_cap = 0;  /* allocated capacity of the cache array */

    /* Initialise the cache with the first five terms */
    if (cache_len == 0) {
        cache_cap = 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failed, fallback to 0 */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 1;
        cache_len = 5;
    }

    /* If the requested term is already cached, return it immediately */
    if ((size_t)n + 1 <= cache_len) {
        return cache[n];
    }

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) {
            new_cap = new_cap * 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* reallocation failed, fallback to 0 */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i < 5) {
            val = cache[i];          /* already initialised */
        } else {
            val = 4 * cache[i-1]      /* 4*a(n-1) */
                - 6 * cache[i-2]      /* -6*a(n-2) */
                + 4 * cache[i-3]      /* +4*a(n-3) */
                -   cache[i-4]        /* -a(n-4) */
                + 2 * cache[i-5];     /* +2*a(n-5) */
        }
        cache[i] = val;
    }

    cache_len = (size_t)n + 1;  /* now indices 0..n are available */
    return cache[n];
}
