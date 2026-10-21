/*
 * A098358  Multiplication table of the triangular numbers read by
 *          antidiagonals.
 * https://oeis.org/A098358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the antidiagonal reading of the rectangular table *
 *            a(m,n) = m*(m+1)*n*(n+1)/4 (m,n ≥ 1). For an index n, * let r be
 *            the unique integer with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. * Let c =
 *            n - r*(r+1)/2. Then the value is * a(n) = ( (c+1)*(c+2)/2 ) * (
 *            (r+1-c)*(r+2-c)/2 )
 * Range:     u8 n <= 47, u16 n <= 963, u32 n <= 1274, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t find_row(idx_t n) {
    idx_t low = 0, high = n;          /* r never exceeds n */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if ((idx_t)t <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;                       /* r such that T(r) ≤ n */
}

seq_t a(idx_t n) {
    /* Determine coordinates (m, k) in the triangular table */
    idx_t r = find_row(n);                     /* row (starting from 0)   */
    unsigned __int128 tri_r = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - (idx_t)tri_r;                /* column within row r     */
    idx_t m = c + 1;                           /* m = c + 1 (starting at 1)   */
    idx_t k = r + 1 - c;                       /* n (in the table) = r+1-c     */

    /* Compute (m*(m+1)/2) * (k*(k+1)/2) using seq_t arithmetic */
    seq_t m_seq = (seq_t)m;
    seq_t k_seq = (seq_t)k;
    seq_t t1 = m_seq * (m_seq + 1) / 2;
    seq_t t2 = k_seq * (k_seq + 1) / 2;
    return t1 * t2;
}
