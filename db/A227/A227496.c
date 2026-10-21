/*
 * A227496  The Wiener index of the nanostar dendrimer defined pictorially as
 *          NS_3 in the Ashrafi et al. references.
 * https://oeis.org/A227496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -446 + 2^n*(5338 - 208*n) + 4^n*(1300 + 10816*n)
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 23, u128 n <= 54,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined for n < 1 */

    seq_t nn  = (seq_t)n;                   /* cast index to seq_t */
    seq_t p2  = pow2(n);                    /* 2^n */
    seq_t p4  = p2 * p2;                    /* 4^n = (2^n)^2 */

    /* compute the terms using only seq_t arithmetic to avoid
       intermediate overflows as long as the final result fits
       in seq_t. */
    seq_t term = p2 * 5338          /* 2^n * 5338          */
              + p4 * 1300           /* 4^n * 1300          */
              + p4 * 10816 * nn;    /* 4^n * 10816 * n    */

    term -= p2 * 208 * nn;          /* subtract 2^n * 208 * n    */
    term -= 446;                    /* subtract constant 446     */

    return term;
}
