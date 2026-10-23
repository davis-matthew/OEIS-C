/*
 * A104792  Triangle T(n,k) = A000330(n-k), n>=1, 0<=k<n, read by rows.
 * https://oeis.org/A104792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   square_pyramid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/square_pyramid.h"
/* Triangle A104792: T(n,k) = A000330(n-k) read by rows.
   For a flat index n (1‑based) we find the corresponding row and column,
   compute m = row - col, and return the square‑pyramidal number
   A000330(m) = m(m+1)(2m+1)/6.  Operations are performed with factor
   cancellation to avoid overflow. */

seq_t a(idx_t n)
{
    /* Find the smallest row such that 1+2+⋯+row >= n. */
    idx_t row = 1;
    idx_t sum = 1;          /* triangular number T_row */
    while (sum < n) {
        ++row;
        sum += row;        /* sum = row*(row+1)/2 */
    }

    /* Column within the row (0‑based). */
    idx_t prev = sum - row;            /* triangular number of previous row (row-1) */
    idx_t col = n - prev - 1u;         /* 0‑based column index */

    /* m = row - col = n - k in the triangle definition. */
    idx_t m = row - col;

    return square_pyramid(m);
}
