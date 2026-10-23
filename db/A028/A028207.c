/*
 * A028207  Expansion of 1/((1-6x)(1-7x)(1-10x)(1-11x)).
 * https://oeis.org/A028207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) - 110*a(n-2) + 7^(n+1) - 6^(n+1); initial
 *            values: a(0) = 1, a(1) = 34
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)34;

    seq_t a_prev2 = (seq_t)1;      /* a(0) */
    seq_t a_prev1 = (seq_t)34;     /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = ((seq_t)21) * a_prev1;
        seq_t term2 = ((seq_t)110) * a_prev2;

        seq_t pow7 = big_pow((seq_t)7, (seq_t)(i + 1));
        seq_t pow6 = big_pow((seq_t)6, (seq_t)(i + 1));

        seq_t a_curr = term1 - term2 + pow7 - pow6;

        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}
