/*
 * A098258  Chebyshev polynomials S(n,531) + S(n-1,531) with Diophantine
 *          property.
 * https://oeis.org/A098258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = 531*a(n-1) - a(n-2),  a(0) = 1,  a(1) = 532 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: store already computed terms */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;   /* number of values in cache */
    static idx_t cache_cap   = 0;   /* allocated capacity */

    /* initialise cache if needed */
    if (cache_len == 0) {
        cache_cap = 4;                     /* start with a few slots */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)532;
        cache_len = 2;
    }

    /* grow cache to ensure we can store up to n */
    while (cache_len <= n) {
        /* double capacity when needed */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap * 2 + 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* compute next term */
        seq_t next = (seq_t)531 * cache[cache_len - 1] - cache[cache_len - 2];
        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[n];
}
