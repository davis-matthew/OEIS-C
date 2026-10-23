/*
 * A120767  a(n) = 2^(n^2) + 3^n.
 * https://oeis.org/A120767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n^2) + 3^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n^2) */
    seq_t term1 = pow_uint(2, n * n);
    /* Compute 3^n    */
    seq_t term2 = pow_uint(3, n);
    return term1 + term2;
}
