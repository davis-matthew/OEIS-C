/*
 * A075503  Stirling2 triangle with scaled diagonals (powers of 8).
 * https://oeis.org/A075503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 46, u64 n <= 138, u128 n <= 383,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n,m) = 8*m*a(n-1,m) + a(n-1,m-1), with a(1,1)=1, a(n,0)=0.
   Flattened triangle: a(k) is the k‑th entry in row‑major order of the
   triangle a(n,m).  Example: 1; 8,1; 64,24,1; ...  */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that triangular(r-1) < n ≤ triangular(r)   */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;
    /* Position within that row (1‑based)                                 */
    idx_t pos = n - (r - 1) * r / 2;

    /* Allocate two rows of length r+1 (index 0 unused)                  */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t) * (r + 1));
    seq_t *cur  = (seq_t *)malloc(sizeof(seq_t) * (r + 1));
    if (!prev || !cur) {
        free(prev);
        free(cur);
        return (seq_t)0;
    }

    /* Base row: n = 1                                               */
    prev[0] = (seq_t)0;           /* unused but initialized                     */
    prev[1] = (seq_t)1;
    for (idx_t i = 2; i <= r; ++i) prev[i] = (seq_t)0;

    /* Build rows 2 … r                                                */
    for (idx_t row = 2; row <= r; ++row) {
        /* m = 1 equivalently: a(row,1) = 8 * prev[1]  */
        cur[1] = prev[1] * ((seq_t)8);

        /* 2 ≤ m ≤ row-1 */
        for (idx_t m = 2; m < row; ++m) {
            cur[m] = prev[m] * ((seq_t)(8 * m)) + prev[m - 1];
        }

        /* m = row: a(row,row) = prev[row-1] */
        cur[row] = prev[row - 1];

        /* swap prev and cur for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    seq_t result = prev[pos];

    /* Clean up */
    free(prev);
    free(cur);
    return result;
}
