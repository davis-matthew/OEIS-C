/*
 * A136336  a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3) for n>3.
 * https://oeis.org/A136336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A136336
   Recurrence: a(0)=1, a(1)=2, a(2)=4, a(3)=7
                a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3)   for n>3
   Formula (valid for n>0): a(n) = (8 + (-2)^n + 21*2^n) / 24
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for arbitrary order calls */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of elements stored */

    /* initialise first four terms on first call or when cache is empty */
    if (cache == NULL) {
        /* allocate at least 4 slots */
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;    /* allocation failure – unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)7;
    }

    /* for n < 4 we already have the value */
    if (n < 4) {
        return cache[(size_t)n];
    }

    /* grow the cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap = (size_t)n + 1;   /* need indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;        /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = new_cap;

        /* compute missing terms using the recurrence */
        for (size_t i = old_cap; i <= (size_t)n; ++i) {
            /*  a(i) = a(i-1) + 4*a(i-2) - 4*a(i-3)  */
            cache[i] = cache[i-1] + (seq_t)4 * cache[i-2] - (seq_t)4 * cache[i-3];
        }
    }

    return cache[(size_t)n];
}
