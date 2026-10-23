/*
 * A034662  Sum of n-th powers of divisors of 20.
 * https://oeis.org/A034662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 2^n + 4^n)*(1 + 5^n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    seq_t term1 = (seq_t)1 + pow2 + pow4;

    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    seq_t term2 = (seq_t)1 + pow5;

    return term1 * term2;
}
