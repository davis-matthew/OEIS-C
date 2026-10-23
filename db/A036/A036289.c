/*
 * A036289  a(n) = n*2^n.
 * https://oeis.org/A036289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*2^n
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    return (seq_t)n * pow2(n);
}
