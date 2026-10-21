/*
 * A088897  T(n,k) = number of ordered pairs of integers (x,y) with x^2/n^2 +
 *          y^2/k^2 <= 1, 1<=k<=n; triangular array, read by rows.
 * https://oeis.org/A088897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = T(r,k) where (r,k) are the row and column indices obtained by
    mapping the flattened triangular array:
        r = smallest integer with r(r+1)/2 >= n
        k = n - (r-1)r/2
    T(r,k) counts integer lattice points (x,y) satisfying
        x^2 / r^2 + y^2 / k^2 <= 1.
*/

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* locate the row r such that r(r+1)/2 >= n */
    idx_t low = 1, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = (mid * (mid + 1)) / 2;
        if (tri >= n) {
            r = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    idx_t prev_tri = ((r - 1) * r) / 2;
    idx_t k = n - prev_tri;

    seq_t count = 0;
    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;
    seq_t r_sq = sr * sr;
    seq_t k_sq = sk * sk;

    for (idx_t x = 0; x <= r; ++x) {
        seq_t x_sq = (seq_t)x * (seq_t)x;
        seq_t t = r_sq - x_sq;          /* >= 0 */
        seq_t u = k_sq * t;
        u /= r_sq;                      /* floor division */
        seq_t y_max = floor_sqrt(u);
        seq_t points = 2 * y_max + 1;
        if (x == 0)
            count += points;
        else
            count += points * 2;        /* account for symmetry in x */
    }
    return count;
}
