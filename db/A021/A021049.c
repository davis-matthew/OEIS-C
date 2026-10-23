/*
 * A021049  Expansion of 1/((1-x)*(1-2*x)*(1-3*x)*(1-10*x)).
 * https://oeis.org/A021049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=16, a(2)=185, a(3)=1940,
   a(n)=16*a(n-1)-71*a(n-2)+116*a(n-3)-60*a(n-4)  for n>=4 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* Grow the cache to be able to index up to n */
    if (cap <= n) {
        size_t newcap = cap ? cap << 1 : 8;
        while (newcap <= (size_t)n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, (newcap + 1) * sizeof(seq_t));
        /* initialise the new elements to 0 */
        for (size_t i = cap; i <= newcap; ++i) cache[i] = (seq_t)0;
        cap = newcap;
    }

    /* Base terms */
    if (cache[0] == (seq_t)0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)185;
        cache[3] = (seq_t)1940;
    }

    /* Compute missing values by the recurrence */
    for (idx_t i = 4; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;          /* already computed */
        cache[i] = cache[i-1] * (seq_t)16
                 - cache[i-2] * (seq_t)71
                 + cache[i-3] * (seq_t)116
                 - cache[i-4] * (seq_t)60;
    }

    return cache[n];
}
