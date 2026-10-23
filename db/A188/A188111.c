/*
 * A188111  Triangle T(n,m) read by rows, [A(x)]^m = Sum_{n>=m} T(n,m)*x^n,
 *          where A(x) satisfies A(x) = x/(1-A(x)-A(x)^2).
 * https://oeis.org/A188111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = row‑wise linearization of the triangle T defined by
 *            T(n,0)=0 (n>0), T(0,0)=1, T(n,n)=1, and for 1 ≤ m < n T(n,m) =
 *            T(n-1,m-1) + T(n,m+1) + T(n,m+2)
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq.h"


seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Column index (1‑based) within that row */
    idx_t k = n - ((r - 1) * r) / 2;

    /* Lazy construction of the triangle up to row r */
    static seq_t **triangle = NULL;
    static idx_t triangle_rows = 0; /* number of rows built (0 … r) */

    while (triangle_rows <= r) {
        idx_t row = triangle_rows;           /* build row 'row' */

        /* Allocate an array of length row+1 (index 0 unused except for row 0) */
        seq_t *row_arr = alloc_seq(row + 1);
        row_arr[0] = 0;                      /* unused slot, set to 0 */

        if (row == 0) {                      /* T(0,0) = 1 */
            row_arr[0] = 1;
        } else {
            /* Compute from right to left: m = row … 1 */
            for (idx_t m = row; ; --m) {
                seq_t val = triangle[row - 1][m - 1];
                if (m + 1 <= row) val += row_arr[m + 1];
                if (m + 2 <= row) val += row_arr[m + 2];
                row_arr[m] = val;
                if (m == 1) break;           /* finished all m ≥ 1 */
            }
        }

        /* Grow the pointer array to hold the new row */
        triangle = (seq_t **)realloc(triangle,
                                     (row + 1) * sizeof(seq_t *));
        triangle[row] = row_arr;
        ++triangle_rows;
    }

    return triangle[r][k];
}
