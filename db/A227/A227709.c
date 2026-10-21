/*
 * A227709  The hyper-Wiener index of the dendrimer D_n defined pictorially in
 *          Fig. 1 of the Heydari et al. reference.
 * https://oeis.org/A227709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12+2^n*(15+45*n+15*n^2) + 4^n*(132+18*n+36*n^2)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 10, u64 n <= 24, u128 n <= 55,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t once for reuse */
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;          /* n^2 */

    /* Compute the first polynomial: 15 + 45*n + 15*n^2 */
    seq_t poly1 = 15;
    poly1 += (seq_t)45 * nn;
    poly1 += (seq_t)15 * nn2;

    /* Compute the second polynomial: 132 + 18*n + 36*n^2 */
    seq_t poly2 = 132;
    poly2 += (seq_t)18 * nn;
    poly2 += (seq_t)36 * nn2;

    /* 2^n and 4^n via the utility pow2 function */
    seq_t two_pow_n   = pow2(n);
    seq_t four_pow_n  = pow2((idx_t)(2 * n));  /* 2^(2n) = 4^n */

    /* Assemble the result: 12 + 2^n*poly1 + 4^n*poly2 */
    seq_t result = 12;
    result += two_pow_n * poly1;
    result += four_pow_n * poly2;

    return result;
}
