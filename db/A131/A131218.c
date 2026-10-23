/*
 * A131218  Array read by antidiagonals: A(n, k) = 1 if and only if the Gray
 *          codes for n and k have no bits in common.
 * https://oeis.org/A131218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) = 1 if the Gray codes of the antidiagonal row and column of
 *            n have no common bit; otherwise 0. The anti‑diagonal coordinates
 *            are: let s be the largest integer with s*(s+1)/2 ≤ n, row = n -
 *            s*(s+1)/2, col = s - row. Then a(n) = 1 iff (row^(row>>1)) &
 *            (col^(col>>1)) == 0
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   max_s_with_tri
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/max_s_with_tri.h"

seq_t a(idx_t n)
{
    idx_t s   = max_s_with_tri(n);                 /* sum index of the diagonal */
    idx_t tri = s * (s + 1) / 2;                   /* number of entries before this diagonal */
    idx_t row = n - tri;                           /* row coordinate on the diagonal */
    idx_t col = s - row;                           /* column coordinate */

    /* Gray code of row and column */
    idx_t g_row = row ^ (row >> 1);
    idx_t g_col = col ^ (col >> 1);

    /* Return 1 if no common bits, else 0 */
    return (seq_t)((g_row & g_col) == 0);
}
