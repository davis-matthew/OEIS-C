/*
 * A024129  a(n) = 11^n - n^2.
 * https://oeis.org/A024129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - n^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t power = big_pow((seq_t)11, (seq_t)n);
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    return power - n2;
}
