/*
 * A123967  Triangle read by rows: T(0,0)=1; for n >= 1 T(n,k) is the
 *          coefficient of x^k in the monic characteristic polynomial of the
 *          tridiagonal n X n matrix with main diagonal 5,5,5,... and sub- and
 *          superdiagonals 1,1,1,... (0 <= k <= n).
 * https://oeis.org/A123967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the n‑th entry of the triangle T(n,k) read row‑by‑row.
 *            Triangle definition: T(0,0) = 1 ; T(n,k) = 0 if k<0 or k>n For n
 *            ≥ 1, T(n,k) = T(n-1,k-1) – 5·T(n-1,k) – T(n-2,k) (0 ≤ k ≤ n)
 *            a(n) = T(r,k) where r is the unique row with r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2 and k = n – r(r+1)/2
 * Range:     i8 n <= 9, i16 n <= 27, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Find the row r containing the index n.  We avoid any floating point or
       square‑root routine.  Cumulative positions of rows are 0,1,3,6,… */
    idx_t r = 0;
    idx_t cumulative = 0;
    while (cumulative + (r + 1) <= n) {
        cumulative += r + 1;
        ++r;
    }
    idx_t k = n - cumulative;          /* column within the row r */

    /* Allocate storage for three consecutive rows of the triangle.
       All arrays have length r+1, which is enough for every row up to r. */
    size_t len = (size_t)r + 1;
    seq_t *prevprev = mem_new(seq_t, len);
    seq_t *prev     = mem_new(seq_t, len);
    seq_t *curr     = mem_new(seq_t, len);
    if (!prevprev || !prev || !curr) {
        mem_free(prevprev); mem_free(prev); mem_free(curr);
        return (seq_t)0;                 /* allocation failed – safe fallback */
    }

    /* Row 0 */
    prev[0] = (seq_t)1;                    /* T(0,0) = 1 */

    if (r == 0) {                          /* a(0) */
        seq_t result = prev[0];
        mem_free(prevprev); mem_free(prev); mem_free(curr);
        return result;
    }

    seq_t result = 0;
    for (idx_t row = 1; row <= r; ++row) {
        /* Compute the current row from the two preceding rows. */
        for (size_t col = 0; col <= (size_t)row; ++col) {
            seq_t left  = 0;
            seq_t mid   = 0;
            seq_t right = 0;

            if (col > 0)                 /* T(row-1, col-1) */
                left = prev[col - 1];

            if (col <= (size_t)row - 1)  /* T(row-1, col) */
                mid = prev[col];

            if (row >= 2)                /* T(row-2, col) */
                right = prevprev[col];

            curr[col] = left - (seq_t)5 * mid - right;
        }

        if (row == r)                    /* desired row reached */
            result = curr[k];

        /* Rotate the row buffers for the next iteration. */
        seq_t *tmp = prevprev;
        prevprev = prev;
        prev = curr;
        curr = tmp;
    }

    mem_free(prevprev); mem_free(prev); mem_free(curr);
    return result;
}
