/*
 * A096198  Triangle read by rows: T(m,n)=A029837(m)+A029837(n), where
 *          (m,n)=(1,1); (2,1), (1,2); (3,1), (2,2), (1,3); ...
 * https://oeis.org/A096198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   T(m,n) = ceil(log2(m)) + ceil(log2(n)) * a(n) is the sequence
 *            obtained by reading the triangle * T row‑wise where row r
 *            contains (m,n) = (r,1),(r-1,2),…,(1,r)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   ceil_log2_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ceil_log2_idx.h"

seq_t a(idx_t n)
{
    /* Find the row r such that the triangular number r*(r+1)/2 is
       the first one >= n.  Use an incremental accumulation to avoid
       overflow.                                                */
    idx_t r = 0;
    idx_t tri = 0;
    while (tri < n)
    {
        r++;
        tri += r;                 /* tri = r*(r+1)/2 after the loop */
    }

    /* tri now holds the triangular number of row r.
       The previous triangular number is tri - r.                     */
    idx_t prev_tri = tri - r;

    /* Zero‑based position inside the row.                           */
    idx_t k = n - prev_tri - 1;

    /* Determine the m and n coordinates of the pair.                */
    idx_t m = r - k;         /* first coordinate          */
    idx_t nn = 1 + k;        /* second coordinate         */

    /* Compute the value of the sequence entry.                       */
    return ceil_log2_idx(m) + ceil_log2_idx(nn);
}
