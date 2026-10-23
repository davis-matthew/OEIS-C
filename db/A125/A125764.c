/*
 * A125764  Array of partial sums of rows of array in A086271, read by
 *          antidiagonals.
 * https://oeis.org/A125764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = k(k+1)/2 + (k(k-1)/2)*(m(m+1)/2-1) + k*(m-1), where k and
 *            m are the row and column indices obtained from the antidiagonal
 *            coordinates of n: d = ceil((1+√(1+8n))/2) prev = (d-1)(d-2)/2
 *            off = n - prev - 1 k = d-1 - off m = 1 + off This corresponds to
 *            the partial sum of the k‑th row of the po...
 * Range:     u8 n <= 58, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, but guard for 0 */

    /* Find the diagonal number d such that d*(d-1)/2 >= n */
    double t = sqrt(1.0 + 8.0 * (double)n);
    idx_t d = (idx_t)((1.0 + t) / 2.0);
    if ((d * (d - 1)) / 2 < n) d++;

    /* Previous diagonal count and offset on the current diagonal */
    idx_t prev = ((d - 1) * (d - 2)) / 2;
    idx_t off  = n - prev - 1;      /* 0‑based offset */

    /* Row and column numbers (k, m) */
    idx_t k = d - 1 - off;          /* row number (>=1) */
    idx_t m = 1 + off;              /* column number (>=1) */

    /* Compute the partial-sum value for row k, column m */
    seq_t k_t = (seq_t)k;
    seq_t m_t = (seq_t)m;

    seq_t t_k = k_t * (k_t + 1) / 2;                 /* triangular number k(k+1)/2 */
    seq_t d_k = k_t * (k_t - 1) / 2;                 /* k(k-1)/2 */
    seq_t sum_i = m_t * (m_t + 1) / 2 - 1;           /* sum of i from 2 to m */

    seq_t result = t_k + d_k * sum_i + k_t * (m_t - 1);
    return result;
}
