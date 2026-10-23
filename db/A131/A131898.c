/*
 * A131898  a(n) = 2^(n+1) + 2*n - 1.
 * https://oeis.org/A131898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) + 2*n - 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n+1) */
    seq_t res = pow2(n + 1);
    /* Add 2*n */
    res += (seq_t)n * 2;
    /* Subtract 1 */
    res -= 1;
    return res;
}
