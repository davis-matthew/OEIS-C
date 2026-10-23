/*
 * A155076  Triangle read by rows. The main diagonal is positive. If rowindex
 *          >= 2*columnindex then -1 else 0.
 * https://oeis.org/A155076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155076 – Triangle read by rows
 *  a(n) = 1                         if the row index equals the column index
 *  a(n) = -1                        if the row index >= 2 * column index
 *  a(n) = 0                         otherwise
 *  The triangle is flattened row by row,
 *  with the first element being a(1)=1.
 *  This implementation computes the row and column for a given n
 *  by progressively accumulating triangular numbers.
 */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, n=0 not used */
    idx_t r = 1;                     /* current row index (1‑based) */
    idx_t tri = 1;                   /* triangular number r*(r+1)/2 */
    while (tri < n) {                /* find the smallest r with tri >= n */
        r++;
        tri = r * (r + 1) / 2;
    }
    /* Column index j = n - triangular number of preceding row */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t j = n - prev_tri;          /* j is 1‑based as well */

    /* Determine the value based on the rule */
    if (j == r) {
        return (seq_t)1;
    } else if (r >= 2 * j) {
        return (seq_t)(-1);
    } else {
        return (seq_t)0;
    }
}
