/*
 * A142589  Square array T(n,m) = Product_{i=0..m} (1+n*i) read by
 *          antidiagonals.
 * https://oeis.org/A142589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 37, u32 n <= 79, u64 n <= 173,
 *            u128 n <= 409, big any n
 * Helpers:   tri_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the antidiagonal reading of the square array
   defined by T(n,m) = ∏_{i=0}^{m} (1 + n*i).
   For a given index n, let k be the largest integer with k*(k+1)/2 ≤ n.
   Then the position in the array is (row = n - k*(k+1)/2,
   col = k - (n - k*(k+1)/2)).  The value is T(row,col).   */
#include <stdint.h>
#include "../utils/tri_floor.h"

seq_t a(idx_t n)
{
    /* Diagonal index */
    idx_t k = tri_floor(n);

    /* Offset within the diagonal */
    __uint128_t tri_k = (__uint128_t)k * (k + 1) / 2;
    idx_t s = (idx_t)(n - tri_k);

    /* Coordinates in the array */
    idx_t row = s;        /* i */
    idx_t col = k - s;    /* j */

    /* T(row,col) = ∏_{m=0}^{col} (1 + row*m) */
    seq_t prod = (seq_t)1;
    if (row == 0) return prod;   /* product is 1 for any column */
    for (idx_t m = 0; m <= col; ++m) {
        prod *= (seq_t)(1 + row * m);
    }
    return prod;
}
