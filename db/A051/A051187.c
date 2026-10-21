/*
 * A051187  Triangle read by rows: T(n, m) = S1(n, m)*8^(n-m), where S1 are
 *          the signed Stirling numbers of first kind A008275 (n >= 1, 1 <= m
 *          <= n).
 * https://oeis.org/A051187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 10, i32 n <= 28, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A051187 (triangle T(n,m) read by rows)

   T(1,1) = 1,  T(n,n) = 1
   T(n,m) = T(n-1,m-1) - 8*(n-1)*T(n-1,m)          for 1 < m < n
   a(n) is the n‑th element of the flattened triangle:
        r = smallest integer with r(r+1)/2 ≥ n
        c = n - r(r-1)/2
        a(n) = T(r,c)
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first term directly */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Find the row r and column c for index n */
    idx_t sum = 1;          /* 1 + 2 + ... + r */
    size_t r = 1;
    while (sum < n) {
        r++;
        sum += (idx_t)r;
    }
    idx_t prev_sum = sum - (idx_t)r;
    size_t c = (size_t)(n - prev_sum);   /* 1‑based column within this row */

    /* Allocate memory for the previous row (row 1) */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t));
    if (!prev) return (seq_t)0;  /* fallback on allocation failure */
    prev[0] = (seq_t)1;          /* T(1,1) */

    /* Build rows from 2 up to r, stopping when we reach the desired row */
    for (size_t row = 2; row <= r; row++) {
        seq_t *curr = (seq_t *)malloc(row * sizeof(seq_t));
        if (!curr) {
            free(prev);
            return (seq_t)0;
        }

        seq_t factor = ((seq_t)8) * ((seq_t)(row - 1));

        /* First element */
        curr[0] = -(factor * prev[0]);

        /* Middle elements, if any */
        if (row > 2) {
            for (size_t m = 1; m < row - 1; m++) {
                curr[m] = prev[m - 1] - factor * prev[m];
            }
        }

        /* Last element */
        curr[row - 1] = (seq_t)1;

        /* If this is the target row, retrieve the value */
        if (row == r) {
            seq_t result = curr[c - 1];
            free(prev);
            free(curr);
            return result;
        }

        /* Prepare for next iteration */
        free(prev);
        prev = curr;
    }

    /* The function should never reach this point */
    return (seq_t)0;
}
