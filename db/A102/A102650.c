/*
 * A102650  a(n) = 4 * floor(28*2^n/15).
 * https://oeis.org/A102650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = 4 * floor(28*2^n/15)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t v = (seq_t)28 * pow2(n);
    return (v / (seq_t)15) * (seq_t)4;
}
