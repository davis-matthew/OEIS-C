/*
 * A009116  Expansion of e.g.f. cos(x) / exp(x).
 * https://oeis.org/A009116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = -2*(a(n‑1)+a(n‑2)), a(0)=1, a(1)=‑1
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 63, i64 n <= 127,
 *            i128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage function as required */
seq_t a(idx_t n)
{
    /* Handle the two base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;

    /* Static cache for memoization: cache[i] stores a(i) for 0 ≤ i < cache_len */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* We need terms up to n, so ensure the cache covers [0 .. n] */
    if (n + 1 > cache_len) {
        /* Allocate or grow the cache */
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp;
        if (cache == NULL) {
            tmp = (seq_t*)malloc(new_len * sizeof(seq_t));
        } else {
            tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        }
        if (!tmp) return 0;                /* Out of memory: return 0 as a fallback */
        cache = tmp;

        /* If this is the first allocation, initialise the base values */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            /* We are sure n+1 ≥ 2 because n >= 2 here */
            cache[1] = (seq_t)-1;
            cache_len = 2;
        }

        /* Compute missing values up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* a(i) = -2 * (a(i-1) + a(i-2))  (avoid a negative literal on the left) */
            cache[i] = -(cache[i-1] + cache[i-2]) * (seq_t)2;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
