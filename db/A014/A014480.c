/*
 * A014480  Expansion of g.f. (1+2*x)/(1-2*x)^2.
 * https://oeis.org/A014480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n+1)*2^n
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t val = pow2(n);
    /* multiply by (2*n+1) */
    seq_t mult = (seq_t)n * 2 + 1;
    val *= mult;
    return val;
}
