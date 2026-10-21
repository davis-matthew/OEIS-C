/*
 * A080875  a(n)*a(n+3) - a(n+1)*a(n+2) = 5, given a(0)=a(1)=1, a(2)=6.
 * https://oeis.org/A080875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = 12*a(n+2) - a(n) Initial values: a(0)=1, a(1)=1,
 *            a(2)=6, a(3)=11 Recurrence order 4 with characteristic
 *            polynomial x^4-12x^2+1 The computation is done iteratively and
 *            results are cached so that arbitrary order of calls is handled
 *            efficiently
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;    /* capacity of cache */
    static size_t cache_len = 0;    /* number of initialized terms   */

    /* initialise cache for first call */
    if (cache == NULL) {
        /* start with space for the first four terms */
        cache_cap = 4;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure – not expected */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)11;
        cache_len = 4;
    }

    /* if requested term is already computed, return it directly */
    if ((idx_t)cache_len > n) {
        return cache[(size_t)n];
    }

    /* extend cache generously (doubling strategy) */
    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) exit(1);              /* allocation failure */
    cache     = tmp;
    cache_cap = new_cap;

    /* compute missing terms up to index n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t term = (seq_t)12 * cache[i-2] - cache[i-4];
        cache[i] = term;
    }
    cache_len = n + 1;

    return cache[(size_t)n];
}
