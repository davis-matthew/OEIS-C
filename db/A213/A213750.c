/*
 * A213750  Rectangular array: (row n) = b**c, where b(h) = h, c(h) =
 *          2*(n-1+h)-1, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = read (b)**(c) with b(h)=h, c(h)=2*(n-1+h)-1, by
 *            antidiagonals of the resulting matrix
 * Range:     u8 n <= 36, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the explicit formula T(row,col) = col*(col+1)*(6*row+2*col-5)/6. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, but guard against 0 */
        return 0;

    /* Find antidiagonal d such that d*(d+1)/2 >= n. */
    idx_t d = 0;
    while ((d * (d + 1ULL) / 2ULL) < n)
        ++d;

    /* Position within diagonal (1‑based). */
    idx_t prev = (d - 1ULL) * d / 2ULL;
    idx_t p    = n - prev;

    /* Convert to matrix coordinates: row = p, col = d - p + 1. */
    idx_t row = p;
    idx_t col = d - p + 1ULL;

    /* Compute T(row,col) with 64‑bit intermediate to avoid overflow. */
    unsigned long long col_u = (unsigned long long)col;
    unsigned long long row_u = (unsigned long long)row;
    unsigned long long num   = col_u * (col_u + 1ULL) *
                               (6ULL * row_u + 2ULL * col_u - 5ULL);

    return (seq_t)(num / 6ULL);
}
