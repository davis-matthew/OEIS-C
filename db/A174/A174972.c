/*
 * A174972  Number of surreal number forms produced by the n-th iteration of
 *          the induction rule.
 * https://oeis.org/A174972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..2^n} i*C(2^n-1,i-1) = (2^n+1)*2^{2^n-2}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute 2^n */
    seq_t pow2n = big_pow((seq_t)2, (seq_t)n);

    /* Exponent for the large power: 2^n - 2 */
    seq_t exponent = pow2n - (seq_t)2;

    /* 2^(2^n - 2) */
    seq_t pow2exp = big_pow((seq_t)2, exponent);

    /* (2^n + 1) * 2^(2^n - 2) */
    seq_t result = pow2exp * (pow2n + (seq_t)1);

    return result;
}
