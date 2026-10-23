/*
 * A141387  Triangle read by rows: T(n,m) = n + 2*m*(n - m) (0 <= m <= n).
 * https://oeis.org/A141387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8*n+1)-1)/2), c = n -
 *            r*(r+1)/2 and T(r,c) = r + 2*c*(r-c)
 * Range:     u8 n <= 261, u16 n <= 9194, u32 n <= 9194, u64 n <= 9194,
 *            u128 n <= 9194, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that offset_row(r) <= n < offset_row(r+1) */
    idx_t r   = 0;
    idx_t off = 0;

    /* offset_row(k) = k*(k+1)/2.  Compute r by incremental scan. */
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }
    off = r * (r + 1) / 2;

    /* Column c (m in the formula) */
    idx_t m = n - off;

    /* Convert to seq_t and compute the triangle value */
    seq_t r_t = (seq_t)r;
    seq_t m_t = (seq_t)m;

    seq_t val = r_t + (seq_t)2 * m_t * (r_t - m_t);
    return val;
}
