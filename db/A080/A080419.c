/*
 * A080419  Triangle of generalized Chebyshev coefficients.
 * https://oeis.org/A080419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A080419
 * Triangle of generalized Chebyshev coefficients.
 * a(n) is the row‑major enumeration of T(r,c):
 *   r = ⌈(√(8n+1)−1)/2⌉,  c = n − r(r−1)/2
 * Triangle T is defined by:
 *   T(1,1) = 1,
 *   T(r,r) = 1,
 *   T(r,1) = (r+2)·3^(r-2)   (r ≥ 2),
 *   T(r,c) = T(r-1,c-1) + 3·T(r-1,c)   for 2 ≤ c ≤ r-1.
 * The function below computes a(n) using this recurrence; it is
 * independent of call order and works for any n ≥ 1.
 */

#include <stdbool.h>

#define MAX_ROWS 200  // sufficient for n ≤ 10000 (triangular numbers)

seq_t a(idx_t n)
{
    static seq_t pow3[MAX_ROWS];
    static bool pow3_init = false;

    if (!pow3_init) {
        pow3[0] = 1;
        for (idx_t i = 1; i < MAX_ROWS; ++i)
            pow3[i] = pow3[i - 1] * 3;
        pow3_init = true;
    }

    /* Arrays to hold previous and current row of the triangle. */
    seq_t prev[MAX_ROWS];
    seq_t curr[MAX_ROWS];

    idx_t total = 0;          // number of terms processed so far
    idx_t row;                // current row number (starting at 1)

    for (row = 1; ; ++row) {
        /* Build row 'row' into curr[0 .. row-1] */
        if (row == 1) {
            curr[0] = 1;
        } else {
            /* First element T(row,1) = (row+2)·3^(row-2) */
            curr[0] = (seq_t)(row + 2) * pow3[row - 2];

            /* Elements 2 .. row-1 */
            for (idx_t k = 2; k <= row - 1; ++k)
                curr[k - 1] = prev[k - 2] + 3 * prev[k - 1];

            /* Last element T(row,row) = 1 */
            curr[row - 1] = 1;
        }

        /* Does the desired index lie in this row? */
        if (total + row >= n) {
            idx_t offset = n - total - 1;   // 0‑based offset in curr
            return curr[offset];
        }

        /* Prepare for next iteration: copy current row into prev. */
        for (idx_t i = 0; i < row; ++i)
            prev[i] = curr[i];

        total += row;   // advance count of processed terms
    }
}
