/*
 * A198852  a(n) = 4*8^n - 1.
 * https://oeis.org/A198852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*8^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t res = pow_uint(8, n);
    res = res * (seq_t)4;
    return res - (seq_t)1;
}
