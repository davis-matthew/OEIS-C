/*
 * A121721  Triangle read by rows: T[n, m] = Sum[m^3 - 3*m^2*k + 3*m*k^2 -
 *          k^3, {k, 0, n - 1}] + m^4.
 * https://oeis.org/A121721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS sequence A121721 – Triangle read by rows.
 *
 * Formula (as implemented):
 *   For row index r ≥ 1 and column index c (1 ≤ c ≤ r) we define
 *       T[r‑1, c‑1] = ∑_{k=0}^{r‑2} ((c‑1)^3 − 3(c‑1)^2 k + 3(c‑1) k² − k³) + (c‑1)⁴
 *   The sequence is read by rows, i.e. a(1)=1 and for n ≥ 2
 *       a(n) = T[r‑1, m] where
 *           r = smallest integer with r(r+3)/2 + 1 ≥ n    (row number)
 *           m = n − (1 + (r‑2)(r+1)/2) − 1              (column, 0‑based)
 *
 * The implementation follows the above mapping exactly.
 */

#include <stdint.h>   /* for uint64_t and standard integer types */

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;          /* first element of the sequence */

    /* Size of the block (row) after the first element */
    idx_t Np = n - 1;

    /* Find smallest k such that k*(k+3)/2 >= Np  (k >= 1) */
    idx_t k = 1;
    while ((k * (k + 3)) / 2 < Np)
        ++k;

    /* Compute cumulative count up to the previous row */
    idx_t cum_prev = 1 + ((k - 1) * (k + 2)) / 2;   /* 1 + (k‑1)(k+2)/2 */

    /* Column index (0‑based) within the row */
    idx_t m = n - cum_prev - 1;

    /* Compute the value of T[k, m] */
    seq_t m_seq = (seq_t)m;
    seq_t sum = 0;
    for (idx_t i = 0; i < k; ++i) {
        seq_t diff = m_seq - (seq_t)i;
        seq_t diff3 = diff * diff * diff;
        sum += diff3;
    }
    seq_t m4 = m_seq * m_seq * m_seq * m_seq;
    sum += m4;
    return sum;
}
