/*
 * A173588  T(n,k) = (k^n)*U(n, (1/k + k)/2), where U(n,x) is the n-th
 *          Chebyshev polynomial of the second kind, square array read by
 *          antidiagonals upward (n >= 0, k >= 1).
 * https://oeis.org/A173588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(row,k) where (row,k) are the coordinates obtained by
 *            reading the 2‑D array of T(row,k) by upward antidiagonals.
 *            Recurrence: T(0,k) = 1 T(1,k) = k^2 + 1 T(n+1,k) =
 *            (k^2+1)*T(n,k) - k^2*T(n-1,k) for n ≥ 1
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the diagonal d such that the total number of elements up
       to that diagonal exceeds n.  The cumulative number of elements
       up to diagonal d (inclusive) is (d+1)(d+2)/2.  Since n is small
       (≤ 10000 in the test data) a simple linear search is fast. */
    idx_t d = 0;
    idx_t cumulative = 1;            /* elements up to diagonal d */
    while (n >= cumulative) {
        ++d;
        cumulative += d + 1;          /* add size of next diagonal */
    }

    /* Elements before this diagonal */
    idx_t prev = cumulative - (d + 1);
    idx_t offset = n - prev;         /* 0 ≤ offset ≤ d */
    idx_t row = d - offset;          /* row index (0‑based) */
    idx_t col = offset + 1;          /* column index (k ≥ 1) */

    /* Compute T(row,col) by the recurrence. */
    if (row == 0) {
        return (seq_t)1;
    }

    seq_t col_s  = (seq_t)col;
    seq_t col2   = col_s * col_s;          /* k^2 */

    if (row == 1) {
        return col2 + (seq_t)1;
    }

    seq_t t0 = (seq_t)1;                   /* T(0,k) */
    seq_t t1 = col2 + (seq_t)1;            /* T(1,k) */
    seq_t k2p1 = col2 + (seq_t)1;          /* (k^2+1) */

    for (idx_t i = 2; i <= row; ++i) {
        seq_t t2 = k2p1 * t1 - col2 * t0;
        t0 = t1;
        t1 = t2;
    }
    return t1;
}
