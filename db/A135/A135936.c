/*
 * A135936  Irregular triangle read by rows: row n gives coefficients of
 *          Boubaker polynomial B_n(x) in order of decreasing exponents
 *          (another version).
 * https://oeis.org/A135936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   * a(n) = flattened triangle T(r,m) * T(r,m) = (-1)^m * ( binom(r
 *            - m, m) - 3 * binom(r - m - 1, m - 1) ) * where r is the row
 *            (starting at 0) and m the index within the row * (0 ≤ m ≤
 *            ⌊r/2⌋). The mapping from n to (r,m) is by * row‑wise linear
 *            traversal of the triangle. The function computes * a(...
 * Range:     i8 n <= 38, i16 n <= 149, i32 n <= 159, i64 n <= 159,
 *            i128 n <= 159, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that cumulative counts of rows 0..r-1 ≤ n <
       cumulative counts of rows 0..r.  Each row r contains floor(r/2)+1
       elements. */
    idx_t r = 0;
    idx_t cum = 0;
    for (;; ++r) {
        idx_t row_len = (r >> 1) + 1;   /* floor(r/2)+1 */
        if (n < cum + row_len) {
            /* n lies in this row */
            idx_t m = n - cum;          /* position inside the row   */
            /* Compute T(r,m) ....................................... */
            seq_t term1 = binom(r - m, m);   /* C(r-m, m)            */
            seq_t term2 = 0;
            if (m > 0) {
                term2 = 3 * binom(r - m - 1, m - 1);   /* 3*C(r-m-1,m-1)  */
            }
            seq_t val = term1 - term2;
            if (m & 1u) {
                /* multiply by (-1) */
                return -val;
            } else {
                return val;
            }
        }
        cum += row_len;  /* accumulate this row's elements */
    }
}
