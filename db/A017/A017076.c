/*
 * A017076  a(n) = (8*n)^12.
 * https://oeis.org/A017076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n)^12
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 5, u128 n <= 203,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 8;
    return pow_int(x, 12);
}
