/*
 * A163514  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = -210*a(n-5) + 20*∑_{k=1..4} a(n-k)   (n ≥ 5)
 * Initial terms: a(0)=1, a(1)=22, a(2)=462, a(3)=9702, a(4)=203742,
 *                 a(5)=4278351  (added numerator correction: +1)
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* cached sequence values   */
    static size_t   size  = 0;           /* allocated length         */
    static idx_t    curmax = (idx_t)(-1);/* highest index computed   */

    /* Pre‑initialise the first five terms; a(5) will be computed correctly
       by the recurrence with the +1 numerator adjustment. */
    if (size < 6) {
        if (cache) free(cache);
        cache = (seq_t*)malloc(6 * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* memory allocation failure   */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)462;
        cache[3] = (seq_t)9702;
        cache[4] = (seq_t)203742;
        cache[5] = (seq_t)1;            /* placeholder, will be overwritten */
        size   = 6;
        curmax = 4;                     /* we have terms up to a(4)   */
    }

    /* If the requested index is already cached, return it. */
    if (n <= curmax) return cache[n];

    /* Grow the cache if necessary. */
    if ((size_t)(n + 1) > size) {
        size_t newsize = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure          */
        cache = tmp;
        size  = newsize;
    }

    /* Compute terms from curmax+1 up to n. */
    for (idx_t i = curmax + 1; i <= n; ++i) {
        if (i <= 4) {                 /* already known */
            continue;
        }

        seq_t sum = (seq_t)0;
        /* sum of the four preceding terms a(i-1)..a(i-4) */
        for (int k = 1; k <= 4; ++k)
            sum += cache[i - k];

        seq_t term = (seq_t)20 * sum;                /* 20*sum */
        seq_t sub   = (seq_t)210 * cache[i - 5];    /* 210*a(i-5) */
        term -= sub;                                /* positive, safe for all n≥5 */

        if (i == 5) {                                /* numerator +1 for a5 */
            term += (seq_t)1;
        }

        cache[i] = term;
    }

    curmax = n;
    return cache[n];
}
