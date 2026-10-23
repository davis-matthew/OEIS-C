/*
 * A169026  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. Cunning recursion from the rational generating
 *            function (t^24 + 2t^23 + … + 2t + 1)/(66t^24 − 11t^23 − … − 11t
 *            + 1) : For any n ≥ 0 a(n) = (n ≤ 24 ? p(n) : 0) + 11 *
 *            Σ_{k=1}^{23} a(n−k) (where a(m)=0 for m<0) − 66 * a(n−24) (term
 *            only when n≥24) with p(n) = 1 for n = 0 or 24, p(n) = 2 f...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached = 0;    /* number of cached terms */

    /* initial allocation */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) abort();
    }

    /* grow cache when needed */
    if (cached <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        /* compute missing terms */
        for (idx_t i = cached; i <= n; ++i) {
            seq_t p;
            if (i == 0 || i == 24)      p = (seq_t)1;
            else if (i <= 23)           p = (seq_t)2;
            else                        p = (seq_t)0;

            /* sum of the previous up to 23 terms */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 23 && i >= k; ++k)
                sum += cache[i - k];

            /* base term */
            seq_t term = p + (seq_t)11 * sum;

            /* subtract 66*a(i-24) when defined */
            if (i >= 24)
                term = term - (seq_t)66 * cache[i - 24];

            cache[i] = term;
        }
        cached = n + 1;
    }
    return cache[n];
}
