/*
 * A238275  a(n) = (4*7^n - 1)/3.
 * https://oeis.org/A238275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*7^n - 1)/3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t val = pow_uint(7, n);
    val = val * 4;
    val = val - 1;
    val = val / 3;
    return val;
}
