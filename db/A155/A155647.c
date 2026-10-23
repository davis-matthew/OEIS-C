/*
 * A155647  a(n) = 9^n+6^n-1^n.
 * https://oeis.org/A155647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n + 6^n - 1^n a(0) = 1, a(1) = 14 Compute the sequence
 *            by fast exponentiation
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow9 = big_pow((seq_t)9, (seq_t)n);
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    return pow9 + pow6 - (seq_t)1;
}
