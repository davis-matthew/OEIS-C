/*
 * A003222  a(n) = 2^(3*n+1) - 2*n*(2*n+1).
 * https://oeis.org/A003222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(3*n+1) - 2*n*(2*n+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   a003222_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a003222_pow2.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t quad = 2 * m + 1;
    quad = quad * m;
    quad = quad * 2;
    return a003222_pow2(3 * n + 1) - quad;
}
