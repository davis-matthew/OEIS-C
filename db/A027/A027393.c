/*
 * A027393  Number of labeled servers of dimension 6.
 * https://oeis.org/A027393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A027393: Number of labeled servers of dimension 6.
 *
 *  EGF:   exp( (x*(360 + 450*x + 400*x^2 + 225*x^3 + 72*x^4 + 10*x^5))/60 )
 *  This yields a sequence a(0)=1, a(1)=6, a(2)=51, a(3)=526, ...
 *
 *  The combinatorial interpretation is that the EGF is exp(C(x)) where
 *  C(x) = c1*x/1! + c2*x^2/2! + … + c6*x^6/6! with c1..c6 =
 *  [6, 15, 40, 90, 144, 120].  Consequently the recurrence
 *
 *      a(0) = 1
 *      a(n) = Σ_{k=1..min(n,6)} c_k * binom(n-1,k-1) * a(n-k)
 *
 *  holds for all n ≥ 1.  Only the first six connected component
 *  sizes occur, so each term requires at most 6 multiplications.
 */

#include <stdlib.h>

static const seq_t C[6] = {(seq_t)6, (seq_t)15, (seq_t)40, (seq_t)90,
                           (seq_t)144, (seq_t)120};

seq_t a(idx_t n)
{
    /* Allocate array for a[0] … a[n] */
    seq_t *a_arr = (seq_t *)malloc(((size_t)(n + 1)) * sizeof(seq_t));
    if (!a_arr) return 0;      /* In practice this never happens for n ≤ 10000 */

    a_arr[0] = (seq_t)1;       /* a(0) = 1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;
        /* k runs from 1 to min(i,6) */
        idx_t maxk = (i < 6) ? i : 6;
        for (idx_t k = 1; k <= maxk; ++k)
        {
            /* Compute binom(i‑1, k‑1) multiplicatively */
            seq_t bin = (seq_t)1;
            for (idx_t t = 1; t <= k - 1; ++t)
            {
                bin *= (seq_t)(i - t);   /* multiply by (i‑t) */
                bin /= (seq_t)t;         /* divide by t */
            }
            /* term = bin * C[k‑1] * a[i‑k] */
            seq_t term = bin * C[k - 1] * a_arr[i - k];
            sum += term;
        }
        a_arr[i] = sum;
    }

    seq_t result = a_arr[n];
    free(a_arr);
    return result;
}
