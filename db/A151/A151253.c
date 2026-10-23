/*
 * A151253  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, 0, 0), (0, 0,
 *          1), (1, 0, 0), (1, 0, 1), (1, 1, 0)}.
 * https://oeis.org/A151253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = Σ_{k=0}^{n} binom(n,k) * Σ_{d=0}^{⌊k/2⌋}
 *            ((k-2d+1)/(k-d+1)) * binom(k,d) * 3^{k-d}
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache_a = NULL;
    static seq_t *cache_b = NULL;
    static seq_t *pow3   = NULL;
    static idx_t  cache_size = 0;   /* number of elements allocated */

    /* allocate or grow caches to cover index n */
    if (n >= cache_size)
    {
        idx_t new_size = n + 1;                /* indices 0..n inclusive */
        /* reallocate arrays */
        cache_a = (seq_t *)realloc(cache_a, new_size * sizeof(seq_t));
        cache_b = (seq_t *)realloc(cache_b, new_size * sizeof(seq_t));
        pow3   = (seq_t *)realloc(pow3,   new_size * sizeof(seq_t));

        /* compute 3^i for i = 0 .. new_size-1 */
        pow3[0] = 1;
        for (idx_t i = 1; i < new_size; ++i)
            pow3[i] = pow3[i-1] * (seq_t)3;

        /* compute b(s) for new indices s = cache_size .. n */
        for (idx_t s = cache_size; s <= n; ++s)
        {
            seq_t b_val = 0;
            seq_t C = 1;                 /* binom(s,0) */
            idx_t maxd = s / 2;
            for (idx_t d = 0; d <= maxd; ++d)
            {
                /* term = binom(s,d) * (s-2d+1)/(s-d+1) * 3^{s-d} */
                seq_t term = C * (seq_t)(s - 2*d + 1);
                term = term / (seq_t)(s - d + 1);
                term = term * pow3[s - d];
                b_val += term;

                /* update binomial coefficient for next d */
                if (d < maxd)
                    C = C * (seq_t)(s - d) / (seq_t)(d + 1);
            }
            cache_b[s] = b_val;
        }

        /* compute a(t) for new indices t = cache_size .. n */
        for (idx_t t = cache_size; t <= n; ++t)
        {
            seq_t a_val = 0;
            seq_t C = 1;                 /* binom(t,0) */
            for (idx_t k = 0; k <= t; ++k)
            {
                a_val += C * cache_b[k];
                if (k < t)
                    C = C * (seq_t)(t - k) / (seq_t)(k + 1);
            }
            cache_a[t] = a_val;
        }

        cache_size = new_size;
    }
    return cache_a[n];
}
