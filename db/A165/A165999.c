/*
 * A165999  Triangle read by rows: T(0,0) = 1, T(n,k) = T(n-1,k-1) + T(n-1,k)
 *          for n > 0, 0 < k <= trinv(n), where trinv(n) =
 *          floor((1+sqrt(1+8*n))/2), and entries outside triangle are 0.
 * https://oeis.org/A165999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   trinv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165999.
   Triangle read by rows: T(0,0) = 1,
   T(n,k) = T(n-1,k-1) + T(n-1,k)   for n>0 , 0<k≤trinv(n).
   a(n) is the flattened sequence: a(n) = T(row, col) where
   row is the unique integer with
   sum_{i=0}^{row-1} (trinv(i)+1) ≤ n < sum_{i=0}^{row} (trinv(i)+1).
   trinv(n) = floor((1+sqrt(1+8*n))/2) is the inverse triangular
   number.  */
#include <stdlib.h>
#include "../utils/trinv.h"

seq_t a(idx_t n)
{
    /* Find the row and column of the term at linear index n. */
    idx_t rem = n;
    idx_t row = 0;
    while (1)
    {
        idx_t len = trinv(row) + 1;
        if (rem < len)
            break;
        rem -= len;
        row++;
    }
    idx_t col = rem;        /* column within the found row */

    /* If we are in the first row, the answer is 1. */
    if (row == 0)
        return (seq_t)1;

    /* Prepare work arrays large enough for the final row. */
    idx_t max_len = trinv(row) + 1;
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t) * max_len);
    seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * max_len);
    if (!prev || !curr)
        return (seq_t)0;   /* allocation failed – unlikely for small n */

    /* Row 0. */
    prev[0] = (seq_t)1;
    idx_t prev_len = 1;

    /* Build rows 1 … row. */
    for (idx_t r = 1; r <= row; r++)
    {
        idx_t curr_len = trinv(r) + 1;
        curr[0] = (seq_t)1;     /* first element of each row is 1 */
        for (idx_t c = 1; c < curr_len; c++)
        {
            seq_t left  = (c - 1 < prev_len) ? prev[c - 1] : (seq_t)0;
            seq_t right = (c < prev_len)     ? prev[c]     : (seq_t)0;
            curr[c] = left + right;
        }
        /* Swap buffers for next iteration. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
        prev_len = curr_len;
    }

    seq_t result = prev[col];
    free(prev);
    free(curr);
    return result;
}
