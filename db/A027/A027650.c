/*
 * A027650  Poly-Bernoulli numbers B_n^(k) with k=-3.
 * https://oeis.org/A027650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*4^n - 6*3^n + 2^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute the required powers using big_pow */
    seq_t p4 = big_pow((seq_t)4, (seq_t)n);   /* 4^n */
    seq_t p3 = big_pow((seq_t)3, (seq_t)n);   /* 3^n */
    seq_t p2 = big_pow((seq_t)2, (seq_t)n);   /* 2^n */

    /* Evaluate the formula */
    const seq_t six = (seq_t)6;
    seq_t result = six * p4 - six * p3 + p2;
    return result;
}
