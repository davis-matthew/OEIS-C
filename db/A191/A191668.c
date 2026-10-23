/*
 * A191668  Dispersion of A016825 (4k+2, k>0), by antidiagonals.
 * https://oeis.org/A191668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = a(i,j) = (8 + (3*floor((4*i+1)/3)-2)*4^j)/12 where (i,j)
 *            are the coordinates of the nth element in the anti‑diagonal
 *            ordering of the infinite table a(i,j) with i,j ≥ 1
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 153, u64 n <= 561,
 *            u128 n <= 2145, big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal d (1‑based) that contains the term n.
       d is the smallest integer such that d*(d+1)/2 ≥ n. */
    idx_t low = 1;
    idx_t high = n;            /* d never exceeds n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;

    /* Triangular number of the previous diagonal. */
    __int128 tri_prev = (__int128)(d - 1) * d / 2;
    idx_t pos = n - (idx_t)tri_prev;   /* 1‑based position on diagonal d */

    /* Coordinates in the table: row i, column j. */
    idx_t i = pos;
    idx_t j = d - pos + 1;

    /* Compute the table value a(i,j). */
    seq_t row = (seq_t)i;
    seq_t floor_val = (seq_t)((4 * row + 1) / 3);
    seq_t factor = 3 * floor_val - 2;
    seq_t pow4j = pow4(j);
    seq_t numerator = 8 + factor * pow4j;
    return numerator / 12;
}
