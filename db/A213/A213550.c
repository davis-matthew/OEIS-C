/*
 * A213550  Rectangular array: (row n) = b**c, where b(h) = h*(h+1)/2, c(h) =
 *          n-1+h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n-th element read by falling antidiagonals from the
 *            convolution array where each row n is the convolution of the
 *            triangular numbers b(h)=h*(h+1)/2 and the arithmetic progression
 *            c(h)=n-1+h. The value at row r and column c is given by the
 *            closed formula below
 * Range:     u8 n <= 15, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 >= n */
    idx_t d = 1;
    while ( (d * (d + 1)) / 2 < n )
        ++d;

    idx_t prev = (d - 1) * d / 2;
    idx_t j    = n - prev;          /* 1-indexed row number on this diagonal */
    seq_t r    = (seq_t) j;          /* row number */
    idx_t c    = d + 1 - j;          /* 1-indexed column number */
    seq_t k    = (seq_t)(c - 1);     /* 0-indexed offset within the row */

    /* Sums needed for the polynomial expression */
    seq_t kk  = k;
    seq_t S1 = kk * (kk + 1) / 2;                         /* sum i          */
    seq_t S2 = kk * (kk + 1) * (2 * kk + 1) / 6;          /* sum i^2        */
    seq_t S3 = (kk * (kk + 1) / 2);
    S3 = S3 * S3;                                         /* (sum i)^2 = sum i^3 */

    seq_t Sum2 = S2 + 3 * S1 + 2 * (kk + 1);               /* sum(i^2+3i+2)   */
    seq_t Sum3 = S3 + 3 * S2 + 2 * S1;                     /* sum(i^3+3i^2+2i) */

    seq_t C1 = (kk + 3) * (kk + 2) * (kk + 1) / 6;         /* binomial(k+3,3) */

    seq_t val = r * C1;
    val += (kk * Sum2 - Sum3) / 2;
    return val;
}
