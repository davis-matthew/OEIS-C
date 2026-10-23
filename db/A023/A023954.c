/*
 * A023954  Expansion of g.f. 1/((1-x)*(1-6*x)*(1-8*x)*(1-9*x)).
 * https://oeis.org/A023954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1)-197*a(n-2)+606*a(n-3)-432*a(n-4) (a(0)=1,
 *            a(1)=24, a(2)=379, a(3)=4974)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t   len   = 0;
    static idx_t   cap   = 0;

    /* initialise cache when needed */
    if (cache == NULL) {
        cap = 16;                         /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) { /* allocation failed */ return (seq_t)0; }
        /* set the first four known values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)24;
        cache[2] = (seq_t)379;
        cache[3] = (seq_t)4974;
        len = 4;
    }

    /* grow the cache if n+1 exceeds current capacity */
    if (n + 1 > cap) {
        idx_t newcap = cap;
        while (newcap < n + 1) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) { /* reallocation failed */ return (seq_t)0; }
        cache = tmp;
        cap = newcap;
    }

    /* fill cache up to n */
    if (n + 1 > len) {
        for (idx_t k = len; k <= n; ++k) {
            seq_t a1 = cache[k - 1];
            seq_t a2 = cache[k - 2];
            seq_t a3 = cache[k - 3];
            seq_t a4 = cache[k - 4];
            /* recurrence computation */
            seq_t val = (seq_t)24 * a1
                      - (seq_t)197 * a2
                      + (seq_t)606 * a3
                      - (seq_t)432 * a4;
            cache[k] = val;
        }
        len = n + 1;
    }
    return cache[n];
}
