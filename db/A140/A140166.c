/*
 * A140166  Triangle read by rows, iterates of X * [1,0,0,0,...]; where X = an
 *          infinite bidiagonal matrix with [1,-2,1,-2,1,...] in the main
 *          diagonal, [1,1,1,...] in the subdiagonal and the rest zeros.
 * https://oeis.org/A140166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 46, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Triangle read by rows of Xⁿ·[1,0,0,…], where X is an
   infinite bidiagonal matrix with diagonal entries [1,-2,1,-2,…]
   and subdiagonal entries 1. a(n) is the n‑th element of that
   triangle (offset 1).  The function constructs enough rows
   to reach the desired index, then returns the appropriate
   entry. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Handle non‑positive n (undefined for this sequence). */
    if (n == 0) return 0;

    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    idx_t sum = 1;
    while (sum < n) {
        r++;
        sum += r;
    }

    /* Position j within row r (0‑based). */
    idx_t prev_sum = sum - r;
    idx_t j = n - prev_sum - 1;

    /* Allocate two buffers of size r (enough for all rows). */
    seq_t *prev = (seq_t *)calloc(r, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(r, sizeof(seq_t));
    if (!prev || !next) {
        /* Allocation failure: return 0 (undefined behaviour otherwise). */
        free(prev); free(next);
        return 0;
    }

    /* Initial vector (row 0) has a single 1. */
    prev[0] = 1;
    idx_t prev_len = 1;          /* current length of prev vector */

    for (idx_t i = 1; i <= r; ++i) {
        /* Compute the i‑th row (length i). */
        for (idx_t k = 0; k < i; ++k) {
            /* Diagonal entry: 1 for k even, -2 for k odd. */
            seq_t d = ((k & 1) ? (seq_t)-2 : (seq_t)1);

            seq_t val = 0;
            if (k < prev_len)
                val = d * prev[k];
            if (k > 0)
                val += prev[k - 1];
            next[k] = val;
        }

        if (i == r) {  /* Desired row reached */
            seq_t result = next[j];
            free(prev);
            free(next);
            return result;
        }

        /* Rotate buffers: next becomes prev for next iteration. */
        seq_t *tmp = prev;
        prev = next;
        next = tmp;
        prev_len = i;
        /* No need to clear 'next'; it will be overwritten in the next loop. */
    }

    free(prev);
    free(next);
    return 0;   /* Fallback; should never be reached. */
}
