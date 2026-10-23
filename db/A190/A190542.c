/*
 * A190542  a(n) = 7^n - 4^n.
 * https://oeis.org/A190542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - 4^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* computed by direct exponentiation using pow_uint() */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t pow7 = pow_uint(7, n);
    seq_t pow4 = pow_uint(4, n);
    return pow7 - pow4;
}
