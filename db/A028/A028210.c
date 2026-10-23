/*
 * A028210  Expansion of 1/((1-6x)(1-8x)(1-9x)(1-10x)).
 * https://oeis.org/A028210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -9*6^n + 125*10^n + 2*8^(n+2) - 3*9^(n+2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Handle n==0 explicitly for safety, though the formula works for all n>=0. */
    if (n == 0) return (seq_t)1;

    /* Compute powers of the bases. */
    seq_t p6  = pow_uint(6, n);
    seq_t p10 = pow_uint(10, n);
    seq_t p8n = pow_uint(8, n);   /* 8^n  */
    seq_t p9n = pow_uint(9, n);   /* 9^n  */

    /* 8^(n+2) = 64 * 8^n, 9^(n+2) = 81 * 9^n. */
    seq_t p8n2 = (seq_t)64 * p8n;
    seq_t p9n2 = (seq_t)81 * p9n;

    /* Sum of positive terms and sum of negative terms. */
    seq_t pos = ((seq_t)125 * p10) + ((seq_t)2 * p8n2);
    seq_t neg = ((seq_t)9 * p6) + ((seq_t)3 * p9n2);

    return pos - neg;
}
