/*
 * A189765  Triangle in which row n has the n*(n+1)/2 elements of the lower
 *          triangular part of the inverse of the n-th order Hilbert matrix.
 * https://oeis.org/A189765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = entry of the lower triangular part of the inverse of the
 *            n‑th order Hilbert matrix, enumerated row‑by‑row
 * Range:     i8 n <= 6, i16 n <= 25, i32 n <= 103, i64 n <= 603,
 *            i128 n <= 2925, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the matrix size m such that the cumulative count of elements up to size m-1
       is less than n and up to m is at least n.  The cumulative count up to size m
       equals m(m+1)(m+2)/6. */
    idx_t m = 1;
    idx_t cum = m * (m + 1) * (m + 2) / 6;
    while (cum < n) {
        ++m;
        cum = m * (m + 1) * (m + 2) / 6;
    }

    /* Position of n within the lower triangle of the m×m inverse Hilbert matrix.
       pos runs from 1 to m(m+1)/2. */
    idx_t prev_cum = (m - 1) * m * (m + 1) / 6;
    idx_t pos = n - prev_cum;

    /* Determine the matrix indices (i,j) (1‑based) corresponding to this position.
       The lower triangle is enumerated row‑by‑row: row i contains i entries. */
    idx_t i;
    idx_t cur = 0;
    for (i = 1; ; ++i) {
        cur += i;
        if (cur >= pos)
            break;
    }
    idx_t prev_i = cur - i;
    idx_t j = pos - prev_i;   /* 1 … i */

    /* Compute the matrix entry M(m,i,j) using the formula from OEIS A189765. */
    seq_t sign = ((i + j) & 1) ? (seq_t)-1 : (seq_t)1;
    seq_t fac  = (seq_t)(i + j - 1);

    seq_t bin1 = binom(m + i - 1, m - j);
    seq_t bin2 = binom(m + j - 1, m - i);
    seq_t bin3 = binom(i + j - 2, i - 1);

    return sign * fac * bin1 * bin2 * bin3 * bin3;
}
