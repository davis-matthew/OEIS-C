/*
 * A014735  Squares of odd numbers in triangle of Eulerian numbers that are
 *          not 1.
 * https://oeis.org/A014735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A014461(n)^2 A014461(n) is the nth odd Eulerian number
 *            (not equal to 1) in the standard reading order of the Eulerian
 *            triangle
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Previous row data */
    size_t prev_size = 1;
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t) * prev_size);
    prev[0] = 1;  /* row 1 contains only 1 */

    idx_t odd_cnt = 0;   /* how many odd entries (≠1) have been seen */

    /* Iterate over rows until we reach the requested odd element */
    while (odd_cnt < n) {
        size_t cur_size = prev_size + 1;   /* next row length */
        seq_t *cur = (seq_t *)malloc(sizeof(seq_t) * cur_size);

        cur[0] = 1;  /* each Eulerian triangle row starts with 1 */

        /* Build the current row from the previous row */
        for (size_t k = 1; k < cur_size; ++k) {
            seq_t left  = (seq_t)(cur_size - k) * prev[k - 1];
            seq_t right = (k < prev_size) ? (seq_t)(k + 1) * prev[k] : 0;
            cur[k] = left + right;
        }

        /* Count odd (≠1) entries in this row in the natural order */
        for (size_t k = 0; k < cur_size; ++k) {
            seq_t val = cur[k];
            if ((val & 1) && val != 1) {
                ++odd_cnt;
                if (odd_cnt == n) {
                    seq_t result = val * val;
                    free(prev);
                    free(cur);
                    return result;
                }
            }
        }

        /* Prepare for next iteration: current row becomes previous */
        free(prev);
        prev = cur;
        prev_size = cur_size;
    }

    /* The loop should always return before reaching here */
    return 0;
}
