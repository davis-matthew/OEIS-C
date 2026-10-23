/*
 * A107980  Triangle read by rows: T(n,k) =
 *          (n+2)*(k+1)*(k+2)*(2*n-k+2)*(2*n-k+3)/24.
 * https://oeis.org/A107980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where row = ⌊(√(8n+1) – 1)/2⌋ (row of the
 *            triangle) col = n – row(row+1)/2, (column within that row) and
 *            T(r, c) = (r+2)*(c+1)*(c+2)*(2r−c+2)*(2r−c+3)/24
 * Range:     u8 n <= 12, u16 n <= 147, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t col = n - r * (r + 1) / 2;

    /* Compute T(r, col) */
    seq_t sr = (seq_t)r;
    seq_t sc = (seq_t)col;
    seq_t term = (sr + 2) * (sc + 1) * (sc + 2) *
                 ((seq_t)(2 * sr - sc + 2)) *
                 ((seq_t)(2 * sr - sc + 3)) / 24;
    return term;
}
