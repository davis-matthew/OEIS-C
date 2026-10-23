/*
 * A140984  Half the number of n X n binary matrices, symmetric under 180
 *          degree rotation, having equal row sums.
 * https://oeis.org/A140984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ½ · Σ_{t=0}^{n} binom(n,t)^{⌊n/2⌋} , for even n a(n) = ½
 *            · Σ_{t=0}^{n} binom(n,t)^{(n‑1)/2} · binom(⌊n/2⌋, ⌊t/2⌋ ) , for
 *            odd n (Here binom(⌊n/2⌋, ⌊t/2⌋) is taken as 0 when ⌊t/2⌋ >
 *            ⌊n/2⌋.)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* not needed for OEIS offset, but safe */
        return 0;

    /* compute binomial coefficients C(n,t) iteratively */
    seq_t C = 1;               /* C(n,0) */
    seq_t sum = 0;

    idx_t exp_rows;            /* exponent for first (non‑middle) rows */
    idx_t m;                   /* m = floor(n/2) */
    seq_t *binom_m = NULL;     /* binom(m, k) for k = 0..m (used when n is odd) */

    if (n & 1)   /* odd n */
    {
        m = n >> 1;                /* floor(n/2) */
        exp_rows = (n - 1) >> 1;   /* (n‑1)/2  */
        /* precompute binom(m, k) for all k = 0..m */
        binom_m = (seq_t*)malloc((m + 1) * sizeof(seq_t));
        binom_m[0] = 1;
        for (idx_t k = 1; k <= m; ++k)
            binom_m[k] = binom_m[k - 1] * (m - k + 1) / k;
    }
    else         /* even n */
    {
        exp_rows = n >> 1; /* n/2 */
    }

    for (idx_t t = 0; t <= n; ++t)
    {
        /* term = binom(n,t)^{exp_rows} */
        seq_t term = pow_seq(C, exp_rows);

        if (n & 1)   /* odd n: multiply by middle coefficient */
        {
            seq_t mid = 0;
            if ((t & 1) == 0)   /* t even */
            {
                idx_t k = t >> 1;          /* t/2 */
                if (k <= m)
                    mid = binom_m[k];
            }
            else                  /* t odd */
            {
                idx_t k = (t - 1) >> 1;    /* (t‑1)/2 */
                if (k <= m)
                    mid = binom_m[k];
            }
            term *= mid;    /* if mid is 0, term becomes 0 */
        }

        sum += term;

        /* update C to C(n,t+1) if not at last iteration */
        if (t < n)
            C = C * (n - t) / (t + 1);
    }

    /* free temporary array */
    if (binom_m)
        free(binom_m);

    return sum / 2;   /* halve the count as specified in the sequence */
}
