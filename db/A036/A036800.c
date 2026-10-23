/*
 * A036800  a(n) = -6 + 2^(n+1)*(3 - 2*n + n^2).
 * https://oeis.org/A036800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -6 + 2^(n+1)*(3 - 2*n + n^2)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 22, u64 n <= 51, u128 n <= 113,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t type */
    seq_t nn = (seq_t)n;

    /* Compute the polynomial part: n^2 - 2*n + 3 */
    seq_t part = nn * nn - (seq_t)2 * nn + (seq_t)3;

    /* Compute 2^(n+1) */
    seq_t pow = pow2(n + 1);

    /* Compute the final result: 2^(n+1) * part - 6 */
    seq_t res = pow * part - (seq_t)6;
    return res;
}
