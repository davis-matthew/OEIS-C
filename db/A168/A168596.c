/*
 * A168596  a(n) = 2*a(n-1) - 1 with a(0)=14.
 * https://oeis.org/A168596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*2^n + 1 (for n >= 0)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    return (seq_t)13 * pow2(n) + 1;
}
