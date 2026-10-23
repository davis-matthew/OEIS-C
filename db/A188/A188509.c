/*
 * A188509  Triangle read by rows: T(n,k) (n >= 1, 1 <= k <= n) is the maximal
 *          number of colors in a vertex coloring of the cube graph Q_n such
 *          that no subgraph Q_k is a rainbow.
 * https://oeis.org/A188509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   T(n,k) = 2^n - 2^(n-k+1) + 1 (n >= 1, 1 <= k <= n); a(m) is the
 *            triangle read by rows, so a(m) = T(r,c) where r is the row of
 *            the m‑th term and c the column in that row
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t m)
{
    /* Find the row r such that r(r+1)/2 >= m > (r-1)r/2 */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < m)
        r++;
    /* Previous row’s last term index */
    idx_t prev = ((r - 1) * r) / 2;
    /* Column inside the row */
    idx_t c = m - prev;
    /* Compute T(r,c) = 2^r - 2^(r-c+1) + 1 */
    seq_t a_r = pow2(r);
    seq_t a_shift = pow2(r - c + 1);
    seq_t result = a_r;
    result -= a_shift;
    result += (seq_t)1;
    return result;
}
