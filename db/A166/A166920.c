/*
 * A166920  a(n) = 2^n - (1 + (-1)^n)/2.
 * https://oeis.org/A166920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - (1+(-1)^n)/2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n);
    if ((n & 1) == 0)
        val -= 1;
    return val;
}
