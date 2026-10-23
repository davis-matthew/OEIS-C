/*
 * A024089  a(n) = 8^n - n.
 * https://oeis.org/A024089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 8^n as 2^(3n) using the helper pow2. */
    idx_t exp = (idx_t)(((__uint128_t)3 * n));
    seq_t pow8 = pow2(exp);
    return pow8 - (seq_t)n;
}
