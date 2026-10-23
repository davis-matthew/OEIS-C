/*
 * A196508  a(n) = 2^n*(n^2 - n + 4) - 4.
 * https://oeis.org/A196508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(n^2 - n + 4) - 4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 23, u64 n <= 52, u128 n <= 114,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t part = nn * nn - nn + 4;
    seq_t pow = pow2(n);
    return pow * part - 4;
}
