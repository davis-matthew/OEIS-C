/*
 * A127360  a(n) = Sum_{k=0..n} binomial(n, floor(k/2))*4^(n-k).
 * https://oeis.org/A127360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A127360
   a(n) = Sum_{k=0..n} binomial(n, floor(k/2)) * 4^(n-k)
   (this implements the direct sum formula, which runs in O(n) time
   and uses only integer arithmetic on seq_t). */
#include <stdint.h>

/* Compute a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    /* base cases from the sequence definition */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)22;

    idx_t maxj = n / 2;          /* floor(n/2) is the largest j used */

    /* compute 4^n once, will be divided by 16 each loop iteration */
    seq_t pow_even = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) pow_even *= (seq_t)4;

    seq_t sum   = (seq_t)0;      /* running total of the sum               */
    seq_t binom = (seq_t)1;      /* binomial coefficient C(n, 0)           */

    for (idx_t j = 0; j <= maxj; ++j)
    {
        /* even k = 2*j term */
        sum += binom * pow_even;

        /* odd k = 2*j + 1 term, if it exists */
        if ((2 * j + 1) <= n)
        {
            seq_t pow_odd = pow_even / (seq_t)4;   /* 4^(n-(2j+1)) */
            sum += binom * pow_odd;
        }

        /* prepare for next j, except after the last iteration */
        if (j < maxj)
        {
            /* next binomial C(n, j+1) = C(n, j) * (n-j) / (j+1) */
            binom = binom * ((seq_t)(n - j)) / ((seq_t)(j + 1));

            /* pow_even for next j is 4^(n-2(j+1)) = pow_even / 16 */
            pow_even = pow_even / (seq_t)16;
        }
    }
    return sum;
}
