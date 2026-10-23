/*
 * A107249  A number triangle with repeated columns of binomial coefficients.
 * https://oeis.org/A107249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(row,col) with T(n,k)=C(floor((2n-k-2)/2),
 *            floor((n-k)/2)), where (row,col) is obtained by linearizing the
 *            triangle row‑wise
 * Range:     u8 n <= 44, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* find the row number r such that
       r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* column index col in the row */
    idx_t col = n - r * (r + 1) / 2;

    /* compute the binomial arguments, using signed arithmetic to handle
       small negative values that only occur for r = 0 or 1. */
    int64_t N_signed = (int64_t)(2 * r - col - 2) / 2;
    int64_t K_signed = (int64_t)(r - col) / 2;

    /* For negative top parameter the only non‑zero binomial is when K==0. */
    if (N_signed < 0)
        return (K_signed == 0) ? (seq_t)1 : (seq_t)0;

    /* cast back to unsigned indices for binom() */
    idx_t N = (idx_t)N_signed;
    idx_t K = (idx_t)K_signed;

    return binom(N, K);
}
