/*
 * A156137  A recursive triangle sequence: A(n,k)=k^2*(A(n - 1, k - 1) + A(n -
 *          1, k)).
 * https://oeis.org/A156137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   A(n,k) = (k+1)^2 * (A(n-1,k-1) + A(n-1,k)) for 1 ≤ k ≤ n-1
 *            A(n,0) = A(n,n) = 1 The sequence a(n) is the triangle read row
 *            by row (0‑based indices)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 40, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* NULL, size_t */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Cached triangular array (row by row).  `tri` holds all entries
       in a linear form: row 0, then row 1, etc. */
    static seq_t *tri = NULL;   /* pointer to allocated data */
    static idx_t rows = 0;      /* number of rows already computed */
    static size_t size = 0;     /* total number of entries stored */

    /* Extend the cache until it contains at least n+1 entries */
    while (size <= (size_t)n) {
        /* New row has length = rows + 1 */
        idx_t  new_row = rows + 1;
        size_t old_size = size;
        size_t new_size = old_size + (size_t)new_row;

        /* Allocate (or grow) the buffer */
        /* use realloc; realloc(NULL, size) behaves like malloc */
        tri = (seq_t *)realloc(tri, new_size * sizeof(seq_t));

        /* Pointers to the new row and the previous row */
        seq_t *row_start  = tri + old_size;
        seq_t *prev_start = tri + (old_size - (size_t)rows);

        /* Boundary values of the new row are always 1 */
        row_start[0]            = (seq_t)1;
        row_start[new_row - 1]  = (seq_t)1;

        /* Interior values: 1 ≤ k ≤ new_row-2  (i is index in new row) */
        for (idx_t i = 1; i + 1 < new_row; ++i) {
            /* k in the recurrence is i+1 because edges are 0–based */
            idx_t k = i + 1;
            seq_t k2 = (seq_t)k * (seq_t)k;                 /* (k+1)^2 */
            seq_t sum = prev_start[i - 1] + prev_start[i];
            row_start[i] = k2 * sum;
        }

        /* Update state for next iteration */
        size = new_size;
        rows = new_row;
    }

    /* Return the required element */
    return tri[(size_t)n];
}
