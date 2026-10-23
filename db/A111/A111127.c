/*
 * A111127  Triangle read by rows:
 *          T(k,s)=(2k-1)(2k+1)binomial(2k-s-1,2k-2s-1)/(2k-2s+1); k>=1,
 *          0<=s<=k-1.
 * https://oeis.org/A111127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the triangle read by rows of * T(k,s) = (2k-1)(2k+1) *
 *            binomial(2k-s-1, 2k-2s-1) / (2k-2s+1), * where rows are indexed
 *            k=1,2,3,... and columns s=0,..,k-1. * The sequence is read
 *            row-wise starting at a(1)
 * Range:     u8 n <= 7, u16 n <= 32, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Compute the triangle entry T(k,s). */
static seq_t compute_T(idx_t k, idx_t s)
{
    /* (2k-1)(2k+1) */
    seq_t term = (seq_t)(2*k - 1) * (seq_t)(2*k + 1);

    /* binomial(2k-s-1, 2k-2s-1) */
    seq_t bin = binomial(2*k - s - 1, 2*k - 2*s - 1);

    term = term * bin;

    /* divide by (2k-2s+1) */
    term = term / (seq_t)(2*k - 2*s + 1);

    return term;
}

/* Return the n-th term of A111127 (OEIS offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */

    idx_t k = 1;                    /* current row */
    idx_t cum = 1;                  /* total terms up to current row */

    /* Find row k such that cum >= n */
    while (cum < n) {
        k++;
        cum += k;
    }

    idx_t prev = cum - k;           /* terms before row k */
    idx_t s = n - prev - 1;         /* column index within the row */

    return compute_T(k, s);
}
