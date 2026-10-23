/*
 * A155543  a(n) = 2*A081294(n).
 * https://oeis.org/A155543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2 for n = 0; a(n) = 4^n for n > 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    return pow_uint((uint64_t)4, n);
}
