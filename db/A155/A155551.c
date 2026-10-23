/*
 * A155551  Triangle read by rows where T(m,n)=2*m*n + m + n - 9.
 * https://oeis.org/A155551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   a(n) = T(m,k) where T(m,k)=2*m*k+m+k-9 Row m has m entries. For
 *            a given 1‑based index n find the smallest m such that m*(m+1)/2
 *            >= n, then let k = n - (m-1)*m/2. The value is 2*m*k+m+k-9
 * Range:     i8 n <= 35, i16 n <= 5050, i32 n <= 5050, i64 n <= 5050,
 *            i128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, a(0) is undefined */
    uint64_t m = 1;
    /* find the row */
    while ((m * (m + 1)) / 2 < n) ++m;
    uint64_t prev = (m - 1) * m / 2; /* entries before this row */
    uint64_t k = n - prev;           /* column within the row, 1‑based */

    seq_t m_seq = (seq_t)m;
    seq_t k_seq = (seq_t)k;

    return 2 * m_seq * k_seq + m_seq + k_seq - 9;
}
