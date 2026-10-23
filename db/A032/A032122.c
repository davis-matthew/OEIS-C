/*
 * A032122  Number of reversible strings with n beads of 5 colors.
 * https://oeis.org/A032122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^n + 5^floor((n+1)/2)) / 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"

seq_t a(idx_t n)
{
    idx_t half = (n + 1) >> 1;
    seq_t pow_n = power5(n);
    seq_t pow_half = power5(half);
    seq_t sum = pow_n + pow_half;
    return sum / (seq_t)2;
}
