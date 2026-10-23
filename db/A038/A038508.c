/*
 * A038508  Expansion of (1-2*x-x^2)/((1-2*x)*(1-2*x+2*x^2)).
 * https://oeis.org/A038508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3), a(0)=1, a(1)=2, a(2)=5
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;   /* number of elements stored */

    /* initialise cache with first three terms */
    if (cache == NULL)
    {
        cache = (seq_t*)malloc(sizeof(seq_t) * 3);
        if (!cache) return (seq_t)0; /* simple error handling */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache_size = 3;
    }

    /* return already computed value */
    if (n < cache_size)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        seq_t val = 4 * cache[i-1] - 2 * cache[i-2] - 4 * cache[i-3];
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (i+1));
        if (!tmp) return (seq_t)0; /* simple error handling */
        cache = tmp;
        cache[i] = val;
    }
    cache_size = n + 1;

    return cache[n];
}
