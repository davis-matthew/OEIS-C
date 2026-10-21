/*
 * A098436  Triangle of 3rd central factorial numbers T(n,k).
 * https://oeis.org/A098436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  T(n, k) = (k+1)^3*T(n-1, k) + T(n-1, k-1),  T(0,0)=1
   a(n) is the flattened triangle row‑wise. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the row (i) and column (j) corresponding to index n. */
    idx_t row = 0;
    idx_t tri = 0;                    /* cumulative length of rows 0 .. row-1 */
    while (tri + row + 1 <= n) {      /* next row fits into the index */
        row++;
        tri += row;                   /* tri = row*(row+1)/2 */
    }
    idx_t col = n - tri;              /* column within the row 0‑based */

    /* Compute the triangular numbers up to the desired row. */
    seq_t *prev = (seq_t*)malloc(sizeof(seq_t));
    if (!prev) return (seq_t)0;       /* allocation failure */
    prev[0] = (seq_t)1;               /* T(0,0) = 1 */

    for (idx_t i = 1; i <= row; ++i) {
        size_t curr_len = (size_t)(i + 1);
        seq_t *curr = (seq_t*)malloc(curr_len * sizeof(seq_t));
        if (!curr) { free(prev); return (seq_t)0; }

        curr[0] = (seq_t)1;           /* T(i,0) = 1 */

        for (idx_t j = 1; j <= i; ++j) {
            seq_t prev_term = (j <= i - 1) ? prev[j] : (seq_t)0;
            seq_t f = (seq_t)(j + 1); /* k+1 */
            seq_t term = prev_term * f * f * f + prev[j - 1];
            curr[(size_t)j] = term;
        }

        free(prev);
        prev = curr;
    }

    seq_t result = prev[(size_t)col];
    free(prev);
    return result;
}
