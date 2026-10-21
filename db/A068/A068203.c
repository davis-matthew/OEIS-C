/*
 * A068203  Chebyshev T-polynomials T(n,15) with Diophantine property.
 * https://oeis.org/A068203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*a(n-1) - a(n-2) Initial values: a(0) = 1, a(1) = 15
 *            The recurrence follows the Pell‐equation description in OEIS
 *            A068203
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation – grows as needed */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise first two terms */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) exit(1);          /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)15;
    }

    /* return if already computed */
    if (n < cache_len) return cache[n];

    /* extend the cache to cover index n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) exit(1);          /* out of memory */
    cache = tmp;

    /* compute missing values using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t t = cache[i - 1] * (seq_t)30;
        cache[i] = t - cache[i - 2];
    }

    cache_len = new_len;
    return cache[n];
}
