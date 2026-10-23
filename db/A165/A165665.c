/*
 * A165665  a(n) = (3*2^n - 2) * 2^n.
 * https://oeis.org/A165665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*2^n - 2) * 2^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 3*2^(2n) */
    seq_t val = pow2(2 * n);
    val = val * 3;

    /* Subtract 2^(n+1) */
    seq_t sub = pow2(n + 1);

    return val - sub;
}
