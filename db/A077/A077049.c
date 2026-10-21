/*
 * A077049  Left summatory matrix, T, by antidiagonals upwards.
 * https://oeis.org/A077049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8929, u16 n <= 8929, u32 n <= 8929, u64 n <= 8929,
 *            u128 n <= 8929, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = 1 if (row % col == 0) where (row, col) is the position of
   n in the anti‑diagonal scan of the infinite matrix T(i,j)=1 if j|i else 0;
   otherwise 0. */
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n = 0, return 0 */

    /* Find the antidiagonal index d such that d(d+1)/2 >= n */
    idx_t d = (_floor_isqrt_u64(8*n - 7) - 1) / 2;
    while ((unsigned __int128)d * (d + 1) / 2 < n) ++d;

    /* Sum of elements in all previous diagonals */
    idx_t prevSum = (unsigned __int128)(d-1) * d / 2;

    /* Offset within this diagonal (0‑based) */
    idx_t offset = n - prevSum - 1;

    /* Coordinates of the cell: row decreases, column increases along the diagonal */
    idx_t row = d - offset;        /* 1 .. d   */
    idx_t col = offset + 1;        /* 1 .. d   */

    /* Return 1 if col divides row, otherwise 0 */
    return (row % col == 0) ? (seq_t)1 : (seq_t)0;
}
