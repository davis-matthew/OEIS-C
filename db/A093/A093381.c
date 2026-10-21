/*
 * A093381  Expansion of (1 - 2*x - 3*x^2 -
 *          4*x^3)/((1-x)*(1-2*x)*(1-3*x)*(1-4*x)).
 * https://oeis.org/A093381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) - 35*a(n-2) + 50*a(n-3) - 24*a(n-4) with
 *            a(0)=1, a(1)=8, a(2)=42, a(3)=186
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* dynamic cache of computed terms   */
    static size_t cache_len = 0;         /* number of terms currently stored */
    static size_t cache_cap = 0;         /* current capacity of the cache    */

    /* Initialise the cache with the first four terms if it hasn't been done */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)42;
        cache[3] = (seq_t)186;
        cache_len = 4;
    }

    /* Return immediately if the requested term is already computed */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Ensure we have enough space for the new terms */
    size_t target = (size_t)n + 1;  /* number of terms needed */
    if (target > cache_cap) {
        /* Double the capacity until it's large enough */
        while (cache_cap < target)
            cache_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t t = cache[i-1] * (seq_t)10;
        t -= cache[i-2] * (seq_t)35;
        t += cache[i-3] * (seq_t)50;
        t -= cache[i-4] * (seq_t)24;
        cache[i] = t;
    }
    cache_len = target;

    return cache[n];
}
