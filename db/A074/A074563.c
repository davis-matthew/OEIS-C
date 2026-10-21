/*
 * A074563  a(n) = 4^n + 5^n + 8^n.
 * https://oeis.org/A074563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 5^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t p4 = pow_uint(4, n);
    seq_t p5 = pow_uint(5, n);
    seq_t p8 = pow_uint(8, n);
    return p4 + p5 + p8;
}
