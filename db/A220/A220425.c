/*
 * A220425  a(n) = n^2 + 2*n + 2^n.
 * https://oeis.org/A220425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*n + 2*n + 2^n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t term = sn * sn + 2 * sn;
    seq_t two_pow = pow2(n);
    return term + two_pow;
}
