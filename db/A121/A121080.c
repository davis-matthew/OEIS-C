/*
 * A121080  a(n) = Sum_{i=0..n} C(n,i)^2*i!*4^i + (1-2^n)*2^(n-1)*n!.
 * https://oeis.org/A121080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} C(n,i)^2 * i! * 4^i + (1-2^n) * 2^(n-1) * n!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Sum part using incremental binomial, factorial and 4^i */
    seq_t sum = 0;
    seq_t binom = 1;          /* C(n,0) */
    seq_t fact_i = 1;         /* i! for current i */
    seq_t pow4_i = 1;         /* 4^i for current i */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* term = binom^2 * fact_i * pow4_i */
        seq_t term = binom * binom;
        term *= fact_i;
        term *= pow4_i;
        sum += term;

        if (i == n) break;   /* no need to update after last iteration */

        /* prepare values for next i+1 */
        binom = binom * (n - i) / (i + 1);  /* C(n,i+1) = C(n,i)*(n-i)/(i+1) */
        fact_i *= (i + 1);                   /* (i+1)! */
        pow4_i *= 4;                         /* 4^(i+1) */
    }

    /* Negative term: -(2^n - 1) * 2^(n-1) * n!  */
    seq_t term2 = 0;
    if (n > 0)
    {
        seq_t pow2n      = pow2(n);            /* 2^n */
        seq_t pow2n_1    = pow2(n - 1);        /* 2^(n-1) */
        seq_t fact_n     = fact_i;             /* n! */
        term2 = (pow2n - 1) * pow2n_1 * fact_n;
    }

    return sum - term2;
}
