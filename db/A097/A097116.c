/*
 * A097116  Expansion of (1-x)/((1-x)^2-3x^3).
 * https://oeis.org/A097116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 3*a(n-3), a(0) = 1, a(1) = 1, a(2) =
 *            1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise the first three terms */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
    }

    if (n < cache_len)
        return cache[n];

    /* extend the cache to contain a(n) */
    idx_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term = cache[i - 1];
        term *= (seq_t)2;                 /* 2*a(i-1) */
        term -= cache[i - 2];              /* - a(i-2) */
        term += cache[i - 3] * (seq_t)3;   /* + 3*a(i-3) */
        cache[i] = term;
    }
    cache_len = new_len;
    return cache[n];
}
