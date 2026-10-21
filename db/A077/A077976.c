/*
 * A077976  Expansion of 1/(1+x+x^2+2*x^3).
 * https://oeis.org/A077976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 18, i16 n <= 36, i32 n <= 73, i64 n <= 146,
 *            i128 n <= 293, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence: a(0)=1, a(1)=-1, a(2)=0,
 * a(n) = -a(n-1) - a(n-2) - 2*a(n-3)   for n ≥ 3
 * (coefficients of 1/(1+x+x²+2x³)).
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored (next free index) */
    static idx_t cache_cap = 0;   /* capacity of cache */

    /* initialise the first three terms the first time the function is called */
    if (cache_len == 0) {
        cache_cap = 8;                           /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;     /* a(0) */
        cache[1] = (seq_t)-1;    /* a(1) */
        cache[2] = (seq_t)0;     /* a(2) */
        cache_len = 3;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* grow the cache until it can hold the requested index */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t t = (seq_t)0;
        if (i >= 1) t += cache[i-1];
        if (i >= 2) t += cache[i-2];
        if (i >= 3) t += (seq_t)2 * cache[i-3];
        cache[i] = -t;
    }
    cache_len = n + 1;

    return cache[n];
}
