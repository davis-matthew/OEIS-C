/*
 * A098730  Sum[i=0..n, Sum[j=0..n, C(n-i,i+j) * C(n-j,i+j) ]].
 * https://oeis.org/A098730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum[i=0..n, Sum[j=0..n, C(n-i,i+j) * C(n-j,i+j) ]]
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* For n = 0 the loop below also works, but we keep the simple case. */
    if (n == 0)
        return 1;

    /* j must satisfy 2*j <= n (otherwise the second inequality i+2*j <= n fails for i=0) */
    for (idx_t j = 0; j <= n / 2; ++j)
    {
        /* Upper bound for i: i <= (n-j)/2 and i <= n-2*j */
        idx_t i_high1 = (n - j) / 2;
        idx_t i_high2 = (n >= 2 * j) ? (n - 2 * j) : 0;
        idx_t i_high = (i_high1 < i_high2) ? i_high1 : i_high2;

        seq_t term = 0;

        for (idx_t i = 0; i <= i_high; ++i)
        {
            if (i == 0)
            {
                /* Initial term for this j */
                term = binomial(n, j) * binomial(n - j, j);
            }
            else
            {
                idx_t prev_i = i - 1;
                idx_t prev_k = prev_i + j;          /* i+j for previous i */

                /* Compute the rational multiplier:
                   term_next = term * ((n-2*prev_i-j)*(n-2*prev_i-j-1)*(n-prev_i-2*j))
                                   / ((n-prev_i)*(prev_k+1)*(prev_k+1))
                */
                idx_t a1 = n - 2 * prev_i - j;      /* >= 0 by construction */
                idx_t a2 = a1 - 1;                  /* may be -1 if a1==0, but that only happens
                                                       when i = i_high and we would not use it */
                idx_t b  = n - prev_i - 2 * j;
                idx_t c  = prev_k + 1;

                seq_t num = term;
                num *= (seq_t)a1;
                num *= (seq_t)a2;
                num *= (seq_t)b;

                seq_t denom = (seq_t)(n - prev_i);
                denom *= (seq_t)c;
                denom *= (seq_t)c;

                term = num / denom;                 /* exact division */
            }

            sum += term;
        }
    }

    return sum;
}
