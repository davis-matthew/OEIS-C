/*
 * A022179  Triangle of Gaussian binomial coefficients [ n,k ] for q = 15.
 * https://oeis.org/A022179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element (starting at 0) of the triangle of *
 *            Gaussian binomial coefficients [n,k]_{q=15} enumerated row‑wise.
 *            * The recurrence used is: * T(0,0) = 1 * T(n,k) = T(n-1,k-1) +
 *            15^k * T(n-1,k) (0 < k < n) * T(n,0) = T(n,n) = 1
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 23, u64 n <= 47, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the row r and column k such that
     * n = r*(r+1)/2 + k  with 0 <= k <= r.
     */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* Base case: row 0 contains only the entry 1. */
    if (r == 0)
        return (seq_t)1;

    /* Pre‑compute powers of 15 up to r. */
    seq_t pow15[155];  /* 155 > maximum possible r (~150 for n <= 10000) */
    pow15[0] = (seq_t)1;
    for (idx_t i = 1; i <= r; ++i)
        pow15[i] = pow15[i - 1] * (seq_t)15;

    /* Allocate two temporary arrays for the current and previous rows. */
    seq_t prev[155];
    seq_t cur[155];

    /* Initialization of row 0. */
    prev[0] = (seq_t)1;

    /* Build rows 1..r. */
    for (idx_t row = 1; row <= r; ++row) {
        for (idx_t col = 0; col <= row; ++col) {
            if (col == 0 || col == row) {
                cur[col] = (seq_t)1;
            } else {
                cur[col] = prev[col - 1] + pow15[col] * prev[col];
            }
        }
        /* Copy current row into prev for the next iteration. */
        for (idx_t col = 0; col <= row; ++col)
            prev[col] = cur[col];
    }

    /* Return the element at column k of the final row. */
    return cur[k];
}
