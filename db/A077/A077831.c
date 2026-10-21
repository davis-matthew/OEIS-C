/*
 * A077831  Expansion of 1/(1-3*x-2*x^2-2*x^3).
 * https://oeis.org/A077831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) + 2*a(n-3), a(0)=1, a(1)=3, a(2)=11
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Initial table for the first three terms. */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)realloc(NULL, sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;        /* Out of memory – unlikely in tests. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)11;
    }

    /* Return the value if we already have it. */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Grow the cached array to accommodate up to n. */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!tmp) return (seq_t)0;                /* Out of memory – unlikely. */
    cache = tmp;

    /* Compute missing terms using the linear recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = 3 * cache[i - 1] + 2 * cache[i - 2] + 2 * cache[i - 3];
    }
    cache_len = new_len;

    return cache[n];
}
