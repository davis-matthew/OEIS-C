/*
 * A230011  Numerators of sum of rows of the inverse of the triangle of Euler
 *          polynomial coefficients P(0,x)=1,
 *          2P(n,x)=(1+x)*[(1+x)^(n-1)+x^(n-1)].
 * https://oeis.org/A230011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 21, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for OEIS A230011
   a(n) is the numerator of the sum of the rows of the inverse of the
   triangle of Euler polynomial coefficients.

   The recurrence follows from the fact that the triangle is lower
   triangular with diagonal entries 1 and coefficients

      P(n,x) = ½[(1+x)^n + x^(n-1) + x^n] .

   If s_n denotes the row sum of the inverse matrix, then

          s_n = 1 - Σ_{k=0}^{n-1} L_{n,k} s_k

   where L_{n,k} are the coefficients of P(n,x).  Writing
   s_n = num[n]/D_n with D_n = 1 for n = 0,1 and
   D_n = 2^⌊log₂ n⌋ for n ≥ 2, we derive an integer recurrence for
   num[n] :

          2·num[n] = 2·D_n
                     - Σ_{k=0}^{n-2} C(n,k) · num[k] · (D_n/D_k)
                     - (n+1)      · num[n‑1] · (D_n/D_{n-1}) .

   This recurrence is evaluated dynamically and stored for faster
   reuse of previously computed values.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static inline idx_t floor_log2_idx(idx_t n)
{
    idx_t r = 0;
    while (n >>= 1) ++r;
    return r;
}

static seq_t *num_cache = NULL;   /* cached numerators */
static idx_t cache_len = 0;       /* last index computed */

/* Ensure numerators up to 'n' are computed and stored in num_cache. */
static void ensure_computed(idx_t n)
{
    if (n < cache_len) return;

    /* Expand cache if necessary */
    num_cache = (seq_t *)realloc(num_cache, sizeof(seq_t) * (n + 1));
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
        {
            num_cache[i] = 1;
        }
        else if (i == 1)
        {
            num_cache[i] = 0;
        }
        else
        {
            idx_t dlog = floor_log2_idx(i);
            seq_t D   = ((seq_t)1) << dlog;          /* D_i */
            seq_t sum2 = 0;                          /* twice the sum of terms */
            seq_t binom = 1;                         /* C(i,0) */
            for (idx_t k = 0; k < i; ++k)
            {
                idx_t dlogk = floor_log2_idx(k);
                idx_t e = dlog - dlogk;             /* power of two multiplier */
                seq_t pow2e = ((seq_t)1) << e;
                seq_t t = num_cache[k] * pow2e;      /* num[k] * 2^e */

                if (k <= i - 2)                     /* C(i,k)/2 term */
                {
                    sum2 += binom * t;
                }
                else if (k == i - 1)                /* (i+1)/2 term */
                {
                    sum2 += ((seq_t)(i + 1)) * num_cache[k] * pow2e;
                }

                /* update binomial for next k */
                if (k != i - 1)
                {
                    binom = binom * (seq_t)(i - k);
                    binom /= (seq_t)(k + 1);
                }
            }
            seq_t sum = sum2 / (seq_t)2;
            num_cache[i] = D - sum;
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    /* Compute and return the numerator a(n). */
    ensure_computed(n);
    return num_cache[n];
}
