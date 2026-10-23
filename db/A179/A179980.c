/*
 * A179980  a(n) = 10*a(n-1) - (-1)^n * n, a(1) = 1.
 * https://oeis.org/A179980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - (-1)^n * n (a(1) = 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>        /* malloc, realloc */

seq_t a(idx_t n)
{
    /* cache of already computed terms; indices 0..cache_size-1 correspond to a(1)..a(cache_size) */
    static seq_t *cache       = NULL;
    static idx_t cache_size   = 0;  /* number of terms stored */
    static idx_t cache_cap    = 0;  /* allocated capacity */

    /* undefined for n == 0, return 0 as a safe fallback */
    if (n == 0)
        return 0;

    /* initialise cache on first call */
    if (cache_size == 0) {
        cache_cap = (n > 1) ? n : 1;          /* at least enough for requested n */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)
            return 0;                         /* allocation failure */
        cache[0] = 1;                         /* a(1) */
        cache_size = 1;
    }

    /* expand capacity if needed */
    if (n > cache_cap) {
        idx_t new_cap = (cache_cap * 2 > n) ? cache_cap * 2 : n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp)
            return 0;                         /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];            /* a(i-1) */
        seq_t val  = prev * 10;
        if (i & 1ULL)                         /* i is odd -> add i */
            val += (seq_t)i;
        else                                 /* i is even -> subtract i */
            val -= (seq_t)i;
        cache[i - 1] = val;                   /* store a(i) */
    }
    cache_size = n;

    return cache[n - 1];
}
