/*
 * A052667  E.g.f. 1/(1-2x-x^4).
 * https://oeis.org/A052667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=8, a(3)=48;
   a(n+4) = (n^4+10*n^3+35*n^2+50*n+24)*a(n) + (2*n+8)*a(n+3) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of computed terms      */
    static size_t  cap   = 0;     /* capacity of the array       */
    static idx_t   last  = (idx_t)-1; /* highest index computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)48;
        last = 3;
    }

    /* grow the cache if needed */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = cap ? cap : 1;
        while (newcap < (size_t)(n + 1))
            newcap <<= 1;            /* double until enough space */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = newcap;
    }

    /* trivial cases */
    if (n <= 3)
        return cache[n];

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        idx_t m = i - 4;                /* recurrence index (n = m) */
        seq_t Km  = (seq_t)m;
        seq_t K2  = Km * Km;
        seq_t K3  = K2 * Km;
        seq_t K4  = K3 * Km;

        seq_t term1 = K4
                    + (seq_t)10 * K3
                    + (seq_t)35 * K2
                    + (seq_t)50 * Km
                    + (seq_t)24;

        seq_t coef3 = (seq_t)2 * Km + (seq_t)8;

        cache[i] = term1 * cache[m] + coef3 * cache[m + 3];
    }

    last = n;
    return cache[n];
}
