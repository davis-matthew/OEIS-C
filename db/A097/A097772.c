/*
 * A097772  Pell equation solutions (13*a(n))^2 - 170*b(n)^2 = -1 with
 *          b(n):=A097771(n), n >= 0.
 * https://oeis.org/A097772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 678*a(n-1) - a(n-2), a(0)=1, a(1)=679
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/* Recursively-memoized iterative computation of A097772. */
seq_t a(idx_t n)
{
    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;   /* array of terms */
    static idx_t cache_len = 0;   /* current allocated size (next index to compute) */

    /* Initialisation: make sure a(0) and a(1) are available. */
    if (cache_len == 0) {
        /* Allocate space for at least two terms. */
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* fallback, should not happen in tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)679;
    }

    /* If the requested index is already cached, return it directly. */
    if (n < cache_len)
        return cache[n];

    /* Expand cache and compute missing terms up to n. */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;          /* fallback, should not happen in tests */
        cache = tmp;
        /* Compute terms from cache_len up to n (inclusive). */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* a(i) = 678*a(i-1) - a(i-2) */
            cache[i] = (seq_t)678 * cache[i-1] - cache[i-2];
        }
        cache_len = new_len;
    }

    return cache[n];
}
