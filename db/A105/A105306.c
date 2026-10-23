/*
 * A105306  Triangle read by rows: T(n,k) is the number of directed
 *          column-convex polyominoes of area n, having the top of the
 *          rightmost column at height k.
 * https://oeis.org/A105306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = T(r,c) where T(n,k) counts directed column-convex
 * polyominoes of area n having the top of the rightmost column at height k.
 * Recurrence for n ≥ 2: 
 *     T(n,k) = 2·T(n−1,k) + T(n−1,k−1) − T(n−2,k−1)
 * with initial values: T(0,0)=1, T(1,0)=T(1,1)=1.
 * The sequence a is obtained by reading the triangle row by row,
 * starting with n = 0.  The first term (n = 1) is T(0,0).
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n) {
    /* Map index n (1‑based) to row r (0‑based) and column c in the triangle */
    idx_t r, c;
    /* Find minimal r such that (r+1)*(r+2)/2 >= n */
    idx_t low = 0, high = 1;
    while ((high + 1) * (high + 2) / 2 < n) ++high;    /* ensure high is large enough */
    while (low < high) {
        idx_t mid = (low + high) / 2;
        if ((mid + 1) * (mid + 2) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    r = low;
    idx_t prev = r * (r + 1) / 2;   /* cumulative terms up to previous row */
    c = n - prev - 1;               /* 0‑based column index */

    /* Static storage for computed rows */
    static seq_t **rows = NULL;
    static idx_t rows_cap = 0;

    /* Allocate storage for rows up to r */
    while (rows_cap <= r) {
        idx_t len = rows_cap + 1; /* row length = r+1 */
        rows = (seq_t**)realloc(rows, (rows_cap + 1) * sizeof(seq_t*));
        rows[rows_cap] = (seq_t*)malloc(len * sizeof(seq_t));

        if (rows_cap == 0) {               /* row 0 */
            rows[0][0] = (seq_t)1;
        } else if (rows_cap == 1) {        /* row 1 */
            rows[1][0] = (seq_t)1;
            rows[1][1] = (seq_t)1;
        } else {                           /* row >= 2 using recurrence */
            for (idx_t k = 0; k <= rows_cap; ++k) {
                seq_t term1 = (k <= rows_cap - 1) ? rows[rows_cap - 1][k] : 0;
                term1 = term1 + term1; /* *2 */
                seq_t term2 = (k > 0) ? rows[rows_cap - 1][k - 1] : 0;
                seq_t term3 = (rows_cap >= 2 && k > 0 && k - 1 <= rows_cap - 2)
                                ? rows[rows_cap - 2][k - 1]
                                : 0;
                rows[rows_cap][k] = term1 + term2 - term3;
            }
        }
        ++rows_cap;
    }

    return rows[r][c];
}
