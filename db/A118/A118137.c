/*
 * A118137  Sum of decimal digits of two successive natural numbers.
 * https://oeis.org/A118137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = digit_sum(n) + digit_sum(n+1)
 * Range:     u8 n <= 346, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t s1 = digit_sum((seq_t)n);
    seq_t s2 = digit_sum((seq_t)(n + 1));
    return s1 + s2;
}
