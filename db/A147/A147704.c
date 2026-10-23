/*
 * A147704  Diagonal sums of Riordan array ((1-2*x)/(1 - 3*x + x^2),
 *          x*(1-x)/(1 - 3*x + x^2)).
 * https://oeis.org/A147704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-3), n>2 ; Initial values: a(0)=1, a(1)=1,
 *            a(2)=3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;          /* allocated capacity */
    static idx_t  max   = (idx_t)-1;  /* highest index computed */

    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* Grow cache if needed */
    if (cap <= n) {
        idx_t newcap = cap ? cap<<1 : 1;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise cache on the first call */
    if (max == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        max = 2;
    }

    /* Compute missing values up to n */
    for (idx_t i = max + 1; i <= n; ++i) {
        seq_t tmp = 3 * cache[i-1];
        cache[i] = tmp - cache[i-3];
    }
    max = n;

    return cache[n];
}
