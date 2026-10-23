/*
 * A193561  Augmentation of the triangle A004736. See Comments.
 * https://oeis.org/A193561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 28, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A193561 – Augmentation of the triangle A004736.  
 * 
 * The triangle w(n,k) is defined for 0 ≤ k ≤ n by the recurrence
 *
 *     w(0,0) = 1
 *     w(n,k) = (n+1-k) * w(n-1,k) + (k+n-1) * w(n-1,k-1)   (n ≥ 1)
 *
 * a(n) is obtained by reading the triangle row‑by‑row:  
 * a(0)=w(0,0),  a(1)=w(1,0), a(2)=w(1,1), a(3)=w(2,0), …
 *
 * The function below computes a(n) for any non‑negative n, using only
 * integer arithmetic.  It allocates two temporary rows, each at most
 * of length r+1 where r is the row containing index n.  For n ≤ 10 000
 * this requires at most about 20 000 operations and a few kilobytes
 * of memory.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the row r containing index n.
     * start_index(r) = r*(r+1)/2.  We need the smallest r such that
     * start_index(r) <= n < start_index(r+1).
     */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column k within that row. */
    idx_t k = n - r * (r + 1) / 2;

    /* Allocate the first row: w(0,0)=1. */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t));
    if (!prev) return 0;          /* allocation failure – unlikely */
    prev[0] = (seq_t)1;

    if (r == 0) {
        seq_t result = prev[0];
        free(prev);
        return result;
    }

    /* Build successive rows up to r. */
    for (idx_t row = 1; row <= r; ++row) {
        size_t curr_len = row + 1;
        seq_t *curr = (seq_t *)malloc(curr_len * sizeof(seq_t));
        if (!curr) {
            free(prev);
            return 0;          /* allocation failure – unlikely */
        }

        /* Compute w(row, j) for j = 0..row. */
        for (size_t j = 0; j < curr_len; ++j) {
            seq_t val_prev  = (j < row)        ? prev[j]     : 0;
            seq_t val_prev_prev = (j > 0)      ? prev[j - 1] : 0;
            seq_t factor1 = (seq_t)(row + 1 - j);
            seq_t factor2 = (seq_t)(j + row - 1);
            curr[j] = factor1 * val_prev + factor2 * val_prev_prev;
        }

        /* Prepare for next iteration. */
        free(prev);
        prev = curr;
    }

    /* prev now holds row r; extract the requested entry. */
    seq_t result = prev[k];
    free(prev);
    return result;
}
