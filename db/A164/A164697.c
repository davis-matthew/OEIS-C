/*
 * A164697  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=12, a(3)=36, a(4)=108, a(5)=324, a(6)=972,
 *            a(7)=2916, a(8)=8742; for n>=9, a(n)= -3*a(n-8) + 2*∑_{k=1}^{7}
 *            a(n-k)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;          /* array of a(i) */
    static idx_t cap = 0;                /* allocated capacity */
    static idx_t last_computed = 8;      /* highest index stored */

    /* initialise base cases if first call */
    if (cache == NULL) {
        /* store a(0) .. a(8) */
        cap = 9;                          /* need indices 0..8 inclusive */
        cache = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)36;
        cache[4] = (seq_t)108;
        cache[5] = (seq_t)324;
        cache[6] = (seq_t)972;
        cache[7] = (seq_t)2916;
        cache[8] = (seq_t)8742;
        last_computed = 8;                /* all base values present */
    }

    /* grow cache if necessary */
    if (n > cap) {
        cap = n;                          /* exact capacity needed */
        cache = (seq_t *)realloc(cache, (cap + 1) * sizeof(seq_t));
    }

    /* if requested value already computed, return it */
    if (n <= last_computed)
        return cache[n];

    /* compute values up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* sum of the previous 7 terms a(i-1) .. a(i-7) */
        for (int k = 1; k <= 7; ++k)
            sum += cache[i - k];
        seq_t term = sum * 2;
        term -= cache[i - 8] * 3;
        cache[i] = term;
    }
    last_computed = n;
    return cache[n];
}
