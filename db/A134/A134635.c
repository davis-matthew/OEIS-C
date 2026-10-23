/*
 * A134635  Row sums of triangle A134634.
 * https://oeis.org/A134635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7*n-1)*a(n-1) - 2*(7*n-8)*a(n-2) + 4*(2*n-5)*a(n-3) all
 *            divided by (n+1) Recurrence tested: a(0)=1, a(1)=2, a(2)=6
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute OEIS sequence A134635: row sums of triangle A134634   */
#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Initial terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;

    /* Sliding window for a(n-3), a(n-2), a(n-1) */
    seq_t a_n_minus_3 = (seq_t)1;
    seq_t a_n_minus_2 = (seq_t)2;
    seq_t a_n_minus_1 = (seq_t)6;
    seq_t result = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* Coefficients are small enough for unsigned int */
        unsigned int c1 = (unsigned int)(7 * i - 1);           /* (7*i-1) */
        unsigned int c2 = (unsigned int)(2 * (7 * i - 8));     /* 2*(7*i-8) */
        unsigned int c3 = (unsigned int)(4 * (2 * i - 5));     /* 4*(2*i-5) */

        seq_t term1   = mul_small(a_n_minus_1, c1);
        seq_t term2   = mul_small(a_n_minus_2, c2);
        seq_t term3   = mul_small(a_n_minus_3, c3);

        seq_t numerator = term1 + term3;   /* term1 + term3 */
        numerator -= term2;                /* numerator = term1 - term2 + term3 */

        result = numerator / ((seq_t)(i + 1));   /* a(i) */

        /* Shift window for next iteration */
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = result;
    }

    return result;
}
