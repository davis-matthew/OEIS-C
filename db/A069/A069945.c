/*
 * A069945  Let M_k be the k X k matrix M_k(i,j)=1/binomial(i+n,j); then
 *          a(n)=1/det(M_(n+1)).
 * https://oeis.org/A069945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^{⌊n/2⌋} * ∏_{i=1}^{2n-1} binomial(i,⌊i/2⌋)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 9,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* n >= 1 as per offset of the sequence */
    seq_t prod = 1;

    /* Compute the product for i = 1 .. 2n-1 in pairs */
    for (idx_t k = 1; k < n; ++k) {
        prod *= binom(2 * k,     k);   /* i = 2k   */
        prod *= binom(2 * k + 1, k);   /* i = 2k+1 */
    }

    /* Sign: (-1)^{floor(n/2)} */
    seq_t sign = 1;
    if ((n >> 1) & 1) sign = -1;

    return prod * sign;
}
