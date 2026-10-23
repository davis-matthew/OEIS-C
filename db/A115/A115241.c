/*
 * A115241  Square array read by antidiagonals: T(n,p) is the number of
 *          linearly independent, homogeneous harmonic polynomials of degree n
 *          in p variables (n,p>=1).
 * https://oeis.org/A115241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The sequence is read from the square array T(n,p) (n,p ≥ 1)
 *            where T(n,p) = (2*n + p - 2) * binomial(n + p - 3, n - 1) / n
 *            The nth term a(n) is the element read along the antidiagonals of
 *            this array (OEIS A115241)
 * Range:     u8 n <= 49, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find antidiagonal length s such that the cumulative count
       of elements up to that diagonal is at least n. */
    idx_t s = 2;        /* current antidiagonal sum n + p   */
    idx_t cum = 1;      /* number of elements up to diagonal s */

    while (cum < n) {
        s++;
        cum += s - 1;   /* diagonal s has s‑1 elements */
    }

    /* Elements before this diagonal */
    idx_t prev   = cum - (s - 1);          /* count of elements before diagonal s */
    idx_t offset = n - prev;              /* 1‑based position inside diagonal s */

    /* Convert offset to matrix coordinates: (row, col) = (n,p) */
    idx_t row = offset;                 /* 1‑based row index */
    idx_t col = s - offset;             /* 1‑based column index */

    /* Compute the required binomial coefficient. */
    seq_t bin = binomial(row + col - 3, row - 1);

    /* Compute the factor (2*n + p - 2). */
    seq_t factor = (seq_t)(2 * row + col - 2);

    /* T(n,p) = factor * bin / row  (row ≥ 1). */
    return (factor * bin) / row;
}
