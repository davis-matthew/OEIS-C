/*
 * A103219  Triangle read by rows: T(n,k) = (n+1-k)*(4*(n+1-k)^2 -
 *          1)/3+2*k*(n+1-k)^2.
 * https://oeis.org/A103219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row,col) where row = ⌊(√(8n+1)-1)/2⌋, col = n -
 *            row*(row+1)/2, and T(r,c) = (r+1-c)*(4*(r+1-c)^2-1)/3 +
 *            2*c*(r+1-c)^2 (triangle read by rows, offset 0)
 * Range:     u8 n <= 14, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   int_sqrt_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_sqrt_idx.h"

seq_t a(idx_t n) {
    /* find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0;
    /* upper bound: sqrt(2*n)+2  (guaranteed to be ≥ required row) */
    idx_t high = int_sqrt_idx(2*n) + 2;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t row = low;
    idx_t col = n - row * (row + 1) / 2;

    /* compute T(row,col) */
    seq_t m = (seq_t)row + 1 - (seq_t)col;   /* m = row+1-col */
    seq_t m2 = m * m;
    seq_t term1 = (m * (4 * m2 - 1)) / 3;    /* first part of the formula */
    seq_t term2 = 2 * (seq_t)col * m2;        /* second part of the formula */
    return term1 + term2;
}
