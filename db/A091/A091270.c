/*
 * A091270  Smallest number having in binary representation a prefix of length
 *          n that is also a suffix of its successor.
 * https://oeis.org/A091270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2^(n+1) - 3 for n > 2; a(0)=0; a(1)=2; a(2)=5
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 2;
    if (n == 2) return 5;
    return pow2(n + 1) - 3;
}
