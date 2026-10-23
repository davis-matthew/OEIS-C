/*
 * A143095  (1, 2, 4, 8, ...) interleaved with (4, 8, 16, 32, ...).
 * https://oeis.org/A143095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 4; a(n) = 2*a(n-2) for n > 1. Equivalently,
 *            a(n) = 2^(n/2) if n is even, a(n) = 2^((n+3)/2) if n is odd
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 60, u64 n <= 124,
 *            u128 n <= 252, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    idx_t e;
    if (n & 1)            /* odd index  →  exponent (n+3)/2 */
        e = (n + 3) / 2;
    else                  /* even index →  exponent n/2   */
        e = n / 2;
    return pow_uint(2, e);
}
