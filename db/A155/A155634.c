/*
 * A155634  a(n) = 7^n + 5^n - 1.
 * https://oeis.org/A155634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n + 5^n - 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t p7 = pow_uint(7, n);
    seq_t p5 = pow_uint(5, n);
    return p7 + p5 - (seq_t)1;
}
