/*
 * A208324  Triangle T(n,k), read by rows, given by (2, -1/2, 1/2, 0, 0, 0, 0,
 *          0, 0, 0, ...) DELTA (4, -2, 0, 0, 0, 0, 0, 0, 0, 0, ...) where
 *          DELTA is the operator defined in A084938.
 * https://oeis.org/A208324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(1+8n)-1)/2), k = n -
 *            r*(r+1)/2 Triangle T is defined by: T(0,0) = 1 T(n,0) = n+1
 *            T(n,1) = 2*T(n,0) + T(n-1,1) T(n,k) = 2*T(n-1,k-1) + T(n-1,k)
 *            for k > 1
 * Range:     u8 n <= 23, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle first term immediately. */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Find the row r that contains index n. */
    idx_t row = 0;
    idx_t cumulative = 0;
    while (1) {
        cumulative += (row + 1);        /* number of terms in rows 0..row */
        if (n < cumulative) break;
        ++row;
    }

    /* k is the column within the found row. */
    idx_t cumulative_before = cumulative - (row + 1); /* terms before this row */
    size_t k = (size_t)(n - cumulative_before);

    /* If we are in row 0, we have already returned. */
    /* Prepare to compute rows up to 'row'. */
    seq_t *prev = NULL;            /* pointer to previous row */
    size_t prev_len = 0;           /* length of previous row */

    /* Row 0 */
    prev = (seq_t *)malloc(sizeof(seq_t) * 1);
    prev[0] = (seq_t)1;
    prev_len = 1;

    /* Compute successive rows up to 'row'. */
    for (idx_t r = 1; r <= row; ++r) {
        size_t curr_len = (size_t)(r + 1);
        seq_t *curr = (seq_t *)malloc(curr_len * sizeof(seq_t));

        /* T(r,0) = r+1 */
        curr[0] = (seq_t)(r + 1);

        /* Compute remaining columns */
        for (size_t c = 1; c < curr_len; ++c) {
            seq_t term;
            if (c == 1) {
                /* T(r,1) = 2*T(r,0) + T(r-1,1) */
                seq_t prev_c1 = (c < prev_len) ? prev[c] : (seq_t)0;
                term = (seq_t)2 * curr[0] + prev_c1;
            } else {
                /* T(r,c) = 2*T(r-1,c-1) + T(r-1,c) */
                seq_t prev_c_minus = prev[c - 1];
                seq_t prev_c = (c < prev_len) ? prev[c] : (seq_t)0;
                term = (seq_t)2 * prev_c_minus + prev_c;
            }
            curr[c] = term;
        }

        /* Free the previous row and move to current. */
        free(prev);
        prev = curr;
        prev_len = curr_len;
    }

    /* Retrieve the requested entry and clean up. */
    seq_t result = prev[k];
    free(prev);
    return result;
}
