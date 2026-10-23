/*
 * A155662  a(n) = 11^n - 7^n + 1.
 * https://oeis.org/A155662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 7^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t p11 = pow_uint(11, n);
    seq_t p7  = pow_uint(7, n);
    return p11 - p7 + (seq_t)1;
}
