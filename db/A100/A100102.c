/*
 * A100102  a(n) = 2^(2*n)-(2*n-1).
 * https://oeis.org/A100102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n) - (2*n - 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t m = 2 * n;             /* exponent 2*n */
    seq_t val = pow2(m);         /* 2^(2*n) */
    seq_t result = val + (seq_t)1; /* add 1 */
    result -= (seq_t)m;          /* subtract 2*n */
    return result;
}
