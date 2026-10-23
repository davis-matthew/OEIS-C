/*
 * A128790  a(n) = n^3*4^n.
 * https://oeis.org/A128790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*4^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 25, u128 n <= 55,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t pow4 = pow_uint(4, n);
    seq_t n3 = sn * sn * sn;
    return n3 * pow4;
}
