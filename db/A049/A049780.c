/*
 * A049780  Array T, read by descending antidiagonals: T(n, k) = k*(2*n + k +
 *          1)/2 for n, k >= 0.
 * https://oeis.org/A049780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the triangle T(n,k) = T(n,k) = k*(2*n +
 *            k + 1)/2 (n,k >= 0) enumerated in descending antidiagonals:
 *            first the antidiagonal d = n + k with k decreasing from d to 0.
 *            For each n, we recover coordinates (i,j) of the triangle entry
 *            and evaluate T(i,j). This runs in O(sqrt(n)) t...
 * Range:     u8 n <= 275, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the diagonal d such that the nth term lies on it.
       The number of terms up to (and including) diagonal d is
       C(d) = (d+1)(d+2)/2.  We search for the smallest d with C(d) > n. */
    idx_t d   = 0;          /* current diagonal (starting from 0) */
    idx_t cum = 1;          /* cumulative term count up to diagonal d */

    while (n >= cum) {
        ++d;
        cum += d + 1;       /* add the number of terms of the next diagonal (d+1) */
    }

    /* Base is the number of terms before diagonal d */
    idx_t base   = d * (d + 1) / 2;          /* sum_{t=0}^{d-1} (t+1) */
    idx_t offset = n - base;                 /* position within diagonal d */
    seq_t i = (seq_t)offset;                 /* row index */
    seq_t j = (seq_t)(d - offset);           /* column index */

    /* Evaluate T(i,j) = j*(2*i + j + 1)/2 */
    return j * ((seq_t)2 * i + j + (seq_t)1) / (seq_t)2;
}
