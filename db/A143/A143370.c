/*
 * A143370  Triangle read by rows: T(n,k) is the number of unordered pairs of
 *          vertices at distance k in the grid P_2 x P_n (1 <= k <= n). P_m is
 *          the path graph on m vertices.
 * https://oeis.org/A143370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle read by rows of the distances in the grid
 *            P2×Pn * Let r be the row: r = ceil((sqrt(8*n+1)-1)/2) * Let k =
 *            n - r*(r-1)/2 (1 ≤ k ≤ r) * Then T(r,k) = 2*(r-k) + (k==1 ? r :
 *            2*(r-k+1)) * a(n) = T(r,k)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   tri_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/tri_sum.h"

seq_t a(idx_t n)
{
    /* find row r such that tri_sum(r-1) < n <= tri_sum(r) */
    idx_t low = 1, high = 1;
    /* grow high until it covers n */
    while (tri_sum(high) < n) high <<= 1;
    /* binary search [low, high] */
    low = high >> 1;
    if (low < 1) low = 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (tri_sum(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = high;                     /* row index */
    seq_t row = (seq_t)r;
    seq_t prev_sum = tri_sum(r - 1);
    seq_t k = (seq_t)(n - prev_sum);     /* column index inside this row */

    /* within-row pairs (dx = 0) */
    seq_t w = 2 * (row - k);

    /* between-row pairs (dx = 1) */
    seq_t across = (k == 1) ? row : (2 * (row - k + 1));

    return w + across;
}
