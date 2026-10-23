/*
 * A151374  Number of walks within N^2 (the first quadrant of Z^2) starting at
 *          (0, 0), ending on the vertical axis and consisting of 2n steps
 *          taken from {(-1, -1), (-1, 0), (1, 1)}.
 * https://oeis.org/A151374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = 2^n * Catalan(n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t two_pow = pow2(n);
    /* Catalan number C_n */
    seq_t cat = catalan(n);
    /* a(n) = 2^n * C_n */
    return two_pow * cat;
}
