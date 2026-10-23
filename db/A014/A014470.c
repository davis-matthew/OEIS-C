/*
 * A014470  Triangular array formed from odd elements to right of middle of
 *          rows of the triangle of Eulerian numbers that are greater than 1.
 * https://oeis.org/A014470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = odd Eulerian numbers >1 that lie strictly to the right of
 *            the middle of each row of the Eulerian triangle
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of A014470.  */
seq_t a(idx_t n)
{
    /* guard against 0, although the problem's offset starts at 1 */
    if (n == 0) return (seq_t)0;

    /* previous row of Eulerian numbers (0‑indexed) */
    seq_t *prev = NULL;
    idx_t prev_len = 0;

    idx_t term_count = 0;
    seq_t result = (seq_t)0;

    for (idx_t row = 1; term_count < n; ++row)
    {
        idx_t cur_len = row;
        seq_t *cur = (seq_t *)malloc(cur_len * sizeof(seq_t));
        if (!cur) { /* allocation failure – return 0 */ return (seq_t)0; }

        /* the first and last element are always 1 */
        cur[0] = (seq_t)1;
        if (row >= 2) cur[row - 1] = (seq_t)1;

        /* compute the interior elements using the recurrence */
        if (row >= 3)
        {
            for (idx_t k = 1; k <= row - 2; ++k)
            {
                cur[k] = ((seq_t)(k + 1)) * prev[k]   /* (k+1)*A(n-1,k)  */
                       + ((seq_t)(row - k)) * prev[k-1];/* (n-k)*A(n-1,k-1)*/
            }
        }

        /* indices that are strictly to the right of the middle */
        idx_t start;
        if (row % 2 == 1)          /* odd row: middle at row/2 */
            start = row / 2 + 1;
        else                       /* even row: middle between row/2-1 and row/2 */
            start = row / 2;

        for (idx_t j = start; j < row; ++j)
        {
            if (cur[j] > (seq_t)1 && (cur[j] & (seq_t)1))
            {
                ++term_count;
                if (term_count == n)
                    result = cur[j];
            }
        }

        /* prepare for the next iteration */
        if (prev) free(prev);
        prev = cur;
        prev_len = cur_len;
    }

    /* free the last allocated row */
    if (prev) free(prev);

    return result;
}
