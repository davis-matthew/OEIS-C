/*
 * A143942  Triangle read by rows: T(n,k) is the number of unordered pairs of
 *          vertices at distance k in a linear chain of n squares joined at
 *          vertices (i.e., joined like <><><>...<>; here <> is a square!); 1
 *          <= k <= 2n.
 * https://oeis.org/A143942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) read by rows of the triangle T(n,k): * T(n,1) = 4n * T(n,2)
 *            = 6n - 4 * For k = 2p+1 (odd, 3 ≤ k ≤ 2n): T(n,k) = 4(n - p) *
 *            For k = 2p (even, 4 ≤ k ≤ 2n): T(n,k) = 5(n - p) + 1 * The
 *            sequence a(n) is obtained by reading all triangle entries * row
 *            by row (row n has 2n values). Indexing starts...
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   T_triangle
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/T_triangle.h"

/* a(n) : n >= 1 is the sequence term */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset is 1, but defensive */

    /* find row r: smallest r with r(r+1) >= n */
    long double disc = sqrtl((long double)4.0L * n + 1.0L);
    idx_t r = (idx_t)((disc - 1.0L) / 2.0L);

    /* adjust r if necessary (at most one increment) */
    while ((seq_t)r * (r + 1) < (seq_t)n)
        ++r;

    /* position k within this row */
    idx_t prev_sum = (r - 1) * r;    /* sum of entries before row r */
    idx_t k = n - prev_sum;          /* 1 ≤ k ≤ 2r */

    return T_triangle(r, k);
}
