/*
 * A028216  Expansion of 1/((1-6x)(1-9x)(1-10x)(1-11x)).
 * https://oeis.org/A028216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) - 110*a(n-2) + 3*9^n - 2*6^n, n >= 2. Initial
 *            values: a(0) = 1, a(1) = 36
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated capacity */
    static idx_t   max_cal = 0; /* largest n computed so far */
    static seq_t  pow6  = (seq_t)6;  /* current 6^i, after last iteration */
    static seq_t  pow9  = (seq_t)9;  /* current 9^i, after last iteration */

    /* initialise cache if first call */
    if (cache == NULL) {
        cap = 2;                     /* store a(0) and a(1) at least */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure – return zero */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)36;
        max_cal  = 1;
    }

    /* if requested value already computed, return it */
    if (n <= max_cal)
        return cache[n];

    /* grow cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;            /* double until sufficient */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;    /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = max_cal + 1; i <= n; ++i) {
        pow6 *= (seq_t)6;                /* pow6 = 6^i   (i >= 2) */
        pow9 *= (seq_t)9;                /* pow9 = 9^i   (i >= 2) */

        seq_t term = (seq_t)21 * cache[i-1];
        term -= (seq_t)110 * cache[i-2];
        term += (seq_t)3 * pow9;
        term -= (seq_t)2 * pow6;

        cache[i] = term;
    }

    max_cal = n;
    return cache[n];
}
