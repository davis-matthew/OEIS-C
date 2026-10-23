/*
 * A013808  a(n) = 18^(4*n + 1).
 * https://oeis.org/A013808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18^(4*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    idx_t exp = n * 4 + 1;          /* compute exponent 4*n + 1 */
    return pow_uint(18, exp);       /* 18 raised to exp, exact for seq_t */
}
