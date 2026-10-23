/*
 * A008297  Triangle of Lah numbers.
 * https://oeis.org/A008297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, nice, easy
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 28, i32 n <= 67, i64 n <= 191,
 *            i128 n <= 530, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of Lah numbers
   a(1,1)=1;  a(n+1,m) = (n+m)*a(n,m)+a(n,m-1)   (m≥1, a(n,0)=0)
   The flattened sequence a(n) is obtained by reading the rows 1,2,3…
   and negating the values of an odd‑indexed row.  Thus
   a(n) = (−1)^{row} · a(row, col) where (row, col) are the indices of
   the triangle cell that corresponds to the flat index n.  This
   implementation computes the requested row on the fly; no global
   state is required and the memory usage is O(√n).  Works for all
   supported seq_t types (fixed‑width or arbitrary‑precision). */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row (r) and column (c) such that the triangular index
       of (r,c) equals n.  The triangular sum T(r)=r(r+1)/2.  */
    idx_t r = 1;
    idx_t t = 1;                 /* current triangular sum T(r)   */
    while (n > t) {
        ++r;
        t += r;                  /* add next row length           */
    }
    idx_t prev_t = t - r;        /* sum up to previous row         */
    idx_t c = n - prev_t;        /* 1‑based column index in row r  */

    /* Compute the triangle up to row r using two row buffers. */
    seq_t *prev = NULL;
    seq_t *curr = NULL;
    for (idx_t rr = 1; rr <= r; ++rr) {
        /* Allocate current row buffer.  Size rr+1 (index 0 unused). */
        curr = (seq_t *)calloc((size_t)(rr + 1), sizeof(seq_t));
        if (curr == NULL) { exit(EXIT_FAILURE); }

        if (rr == 1) {
            curr[1] = 1;                  /* a(1,1)=1 */
        } else {
            for (idx_t m = 1; m <= rr; ++m) {
                seq_t val1 = (m <= rr - 1) ? prev[m] : 0;
                seq_t val2 = (m > 1) ? prev[m - 1] : 0;
                curr[m] = ((seq_t)(rr - 1 + m)) * val1 + val2;
            }
        }

        /* Recycle previous row buffer. */
        free(prev);
        prev = curr;
    }

    /* Get the requested element from the final row. */
    seq_t result = prev[c];

    /* Negate the value if the row index is odd. */
    if (r & 1) {
        result = -result;
    }

    free(prev);
    return result;
}
