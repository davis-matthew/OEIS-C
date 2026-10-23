/*
 * A116669  Triangle, rows tend to A001787, number of edges in n-dimensional
 *          hypercubes.
 * https://oeis.org/A116669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A116669: a(n) is the reverse-ordered antidiagonal of the
   array formed by binomial transforms of the odd-number vectors.
   For row index i (starting at 0) and column j,
   arr[i][j] = Σ_{k=0}^{min(i,j)} binom(j,k) * (2k+1).
   The n‑th term is arr[i][j] where (i,j) is on the k‑th antidiagonal
   with k the row index of the triangle and the element order taken
   from bottom to top.                                                     */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row index k of the triangle such that the first
       (k+1)*(k+2)/2 terms cover n.  k is 0‑based. */
    idx_t t = 0;     /* total terms before current row */
    idx_t k = 0;     /* current row index */
    while (t + k + 1 < n) {
        t += k + 1;
        ++k;
    }

    /* Position within the row (0‑based) */
    idx_t pos = n - t - 1;

    /* Compute array coordinates in reverse order:
       i = row index of array = k - pos,
       j = column index of array = pos        */
    idx_t i = k - pos;
    idx_t j = pos;

    /* Compute arr[i][j] = Σ_{t=0}^{min(i,j)} binom(j,t)*(2t+1) */
    idx_t limit = i < j ? i : j;
    seq_t sum = 0;
    seq_t bin = 1;     /* binom(j,0) */

    for (idx_t t = 0; t <= limit; ++t) {
        seq_t term = bin * ((seq_t)2 * t + 1);
        sum += term;
        if (t < limit) {
            bin = (bin * (j - t)) / (t + 1);
        }
    }

    return sum;
}
