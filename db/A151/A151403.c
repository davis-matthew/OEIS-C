/*
 * A151403  Number of walks within N^2 (the first quadrant of Z^2) starting at
 *          (0,0), ending on the vertical axis and consisting of 2*n steps
 *          taken from {(-1, 0), (-1, 1), (1, 0), (1, 1)}.
 * https://oeis.org/A151403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = 4^n * catalan(n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   catalan, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* 4^n */
    seq_t pow4n = pow_uint(4ULL, n);
    /* Catalan number C_n */
    seq_t catn = catalan(n);
    /* product */
    return pow4n * catn;
}
