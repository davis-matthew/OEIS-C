/*
 * A171390  a(n) = 37*2^(n-1)-1.
 * https://oeis.org/A171390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 37*2^(n-1)-1
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Offset is 1 – if called with n==0 we simply return 0. */
    if (n == 0U) return 0;
    seq_t v = pow2(n - 1);   /* 2^(n-1) */
    v *= (seq_t)37;          /* 37*2^(n-1) */
    v -= (seq_t)1;           /* minus 1 */
    return v;
}
