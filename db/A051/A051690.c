/*
 * A051690  a(n) = (5*n+9)(!^5)/9(!^5), related to A034301 ((5*n+2)(!^5)
 *          quintic, or 5-factorials).
 * https://oeis.org/A051690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) * (5*n + 9), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;    /* largest n cached (inclusive) */

    /* initialise cache with a(0) */
    if (max_cached == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed (unlikely) */
        cache[0] = (seq_t)1;
        max_cached = 0;
    }

    /* if n is already cached, return it */
    if (n <= max_cached) {
        return cache[n];
    }

    /* resize cache to hold up to n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;        /* reallocation failed */
    cache = tmp;

    /* compute new terms iteratively */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t factor = (seq_t)5 * (seq_t)i + 9;
        cache[i] = cache[i - 1] * factor;
    }

    max_cached = n;
    return cache[n];
}
