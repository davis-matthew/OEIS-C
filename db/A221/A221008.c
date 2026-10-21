/*
 * A221008  The Wiener index of the nanostar dendrimer D_3[n] , defined
 *          pictorially in the Alikhani - Iranmanesh reference.
 * https://oeis.org/A221008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -1446 + 2^n*12132 + 4^n*(8820*n - 9660)
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t pow2n = pow2(n);

    /* 4^n = (2^n)^2 */
    seq_t pow4n = pow2n * pow2n;

    /* a(n) = 12132*2^n + (8820*n - 9660)*4^n - 1446 */
    seq_t term = (seq_t)12132 * pow2n;
    seq_t temp = (seq_t)8820 * n - (seq_t)9660;
    term += temp * pow4n;
    term -= (seq_t)1446;

    return term;
}
