/*
 * A020979  Expansion of 1/((1-8*x)*(1-10*x)*(1-11*x)).
 * https://oeis.org/A020979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=29, a(2)=563;
   a(n)=29*a(n-1)-278*a(n-2)+880*a(n-3)   for n>2 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms; works for any seq_t type */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored */
    static size_t cache_cap = 0;   /* capacity of the allocated array */

    /* lazily initialize for the first call */
    if (cache_len == 0) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;               /* memory allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)29;
        cache[2] = (seq_t)563;
        cache_len = 3;
    }

    /* grow the array if required */
    if ((size_t)n + 1 > cache_cap) {
        cache_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return 0;                  /* memory allocation failure */
        cache = tmp;
    }

    /* return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t t1 = (seq_t)29 * cache[i - 1];
        seq_t t2 = (seq_t)278 * cache[i - 2];
        seq_t t3 = (seq_t)880 * cache[i - 3];
        cache[i] = t1 - t2 + t3;             /* exact arithmetic in seq_t */
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
