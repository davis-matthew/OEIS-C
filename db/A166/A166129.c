/*
 * A166129  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A166129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*(a(n-1)+a(n-2)+...+a(n-9)) - 496*a(n-10) + p(n) where
 *            p(0)=1, p(1..9)=2, p(10)=1, and p(n)=0 for n>10
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* caches previously computed values */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t sz    = 0;   /* number of computed terms (>=1) */

    /* initialise capacity */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }
    /* grow cache if necessary */
    while (n >= cap) {
        cap <<= 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* compute missing terms up to n */
    for (idx_t i = sz; i <= n; ++i) {
        seq_t term = 0;

        if (i == 0) {
            term = 1;
        } else {
            /* sum of the previous up to 9 terms */
            seq_t sum9 = 0;
            for (idx_t k = 1; k <= 9 && i >= k; ++k)
                sum9 += cache[i - k];

            term = 31 * sum9;                    /* 31*(a(i-1)+...+a(i-9)) */
            if (i >= 10)
                term -= 496 * cache[i - 10];     /* -496*a(i-10) */

            /* add the inhomogeneous part p(i) */
            int p;
            if (i == 10)
                p = 1;
            else if (i >= 1 && i <= 9)
                p = 2;
            else
                p = 0;
            term += (seq_t)p;
        }

        cache[i] = term;
    }

    sz = n + 1;
    return cache[n];
}
