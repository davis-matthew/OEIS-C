/*
 * A127738  Triangle read by rows: the matrix product A004736 * A127701 of two
 *          triangular matrices.
 * https://oeis.org/A127738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = r + k*(r-k) * where r is the smallest integer with
 *            r*(r+1)/2 >= n, and * k = n - r*(r-1)/2 (column index in the
 *            triangle)
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset is 1, but handle 0 gracefully */

    /* Compute r: solve r*(r+1)/2 >= n  */
    seq_t temp = (seq_t)n * 8 + 1;              /* 8*n + 1 */
    seq_t sq   = floor_sqrt(temp);              /* floor of sqrt(8*n+1) */
    seq_t r    = (sq - 1) / 2;                  /* floor of (sqrt(8n+1)-1)/2 */

    /* Ensure r is the minimal integer satisfying the inequality */
    if (r * (r + 1) / 2 < (seq_t)n)
        r++;

    /* Compute column index k */
    seq_t prev_tri = r * (r - 1) / 2;           /* number of terms before row r */
    seq_t k        = (seq_t)n - prev_tri;

    /* Triangle entry: T(r,k) = r + k*(r-k) */
    return r + k * (r - k);
}
