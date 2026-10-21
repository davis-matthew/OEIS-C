/*
 * A226031  Number A(n,k) of unimodal functions f:[n]->[k*n]; square array
 *          A(n,k), n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A226031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{j=0..k*n-1} C(n+2*j-1,2*j) where (n,k) are the
 *            coordinates on the antidiagonal of the square array A(n,k)
 *            defined by OEIS A226031. This function computes a(n) for any
 *            non‑negative integer n
 * Range:     u8 n <= 23, u16 n <= 40, u32 n <= 85, u64 n <= 222,
 *            u128 n <= 583, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2
       The diagonal length is d+1. */
    idx_t d = 0;
    /* linear search is fine because d is small (<= ~141 for n<=10000) */
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    idx_t t = d * (d + 1) / 2;          /* first index of this diagonal */
    idx_t row = n - t;                  /* n-coordinate on array (n) */
    idx_t col = d - row;                /* k-coordinate on array (k) */

    /* Special case: first row (n==0) -> all entries = 1 */
    if (row == 0)
        return (seq_t)1;

    /* If k==0 the sum is empty -> 0 */
    if (col == 0)
        return (seq_t)0;

    /* Compute the sum Σ_{j=0}^{k*n-1} C(n+2j-1, 2j) */
    idx_t m = row * col;                  /* upper bound for j (exclusive) */
    seq_t sum = 0;
    for (idx_t j = 0; j < m; ++j) {
        sum += binom(row + 2 * j - 1, 2 * j);
    }
    return sum;
}
