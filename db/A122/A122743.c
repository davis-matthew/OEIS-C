/*
 * A122743  Number of normalized polynomials of degree n in GF(2)[x,y].
 * https://oeis.org/A122743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^((n+1)(n+2)/2) - 2^(n(n+1)/2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute the two exponents in idx_t */
    idx_t exp1 = (n + 1) * (n + 2) / 2;
    idx_t exp2 = n * (n + 1) / 2;

    /* compute the powers of two */
    seq_t p1 = pow2(exp1);
    seq_t p2 = pow2(exp2);

    return p1 - p2;
}
