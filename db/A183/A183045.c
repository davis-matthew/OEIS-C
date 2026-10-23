/*
 * A183045  Triangular array, T(i,j)=number of knight's moves to points on
 *          vertical segments (n,-n), (n,-n+1), ... (n,n) on infinite
 *          chessboard.
 * https://oeis.org/A183045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = minimal number of knight moves from (0,0) to the point
 *            (r‑1, y) where r = ceil(sqrt(n)) and y = (n - (r-1)² - 1) -
 *            (r-1)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   knight_moves
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/knight_moves.h"

/* Integer floor square root for an unsigned 64‑bit integer.
   Uses an efficient Newton‑Raphson iteration. */
static inline idx_t floor_isqrt_u64(idx_t n) {
    idx_t x = n;
    if (n == 0) return 0;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n) {
    /* Find the row index r such that (r-1)² < n ≤ r².
       r = ceil(sqrt(n)) = floor_sqrt(n-1)+1 */
    idx_t r = floor_isqrt_u64(n - 1) + 1;

    /* number of elements before this row: (r-1)² */
    idx_t prev_sq = (idx_t)((seq_t)(r - 1) * (seq_t)(r - 1));

    /* offset within the row (0‑based) */
    idx_t t = n - prev_sq - 1;

    /* coordinates of the target point */
    idx_t x = r - 1;                             /* x = n' = r-1 */
    int64_t y_signed = (int64_t)t - (int64_t)(r - 1);

    return knight_moves(x, y_signed);
}
