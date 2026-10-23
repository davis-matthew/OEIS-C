/*
 * A047671  Square array a(n,k) read by antidiagonals: a(n,1)=1, a(1,k)=1,
 *          a(n,k) = 1 + a(n-1,k-1) + a(n-1,k) + a(n,k-1).
 * https://oeis.org/A047671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n,1) = 1,   a(1,k) = 1;
   a(n,k) = 1 + a(n-1,k-1) + a(n-1,k) + a(n,k-1);
   Sequence a(n) is the values a(n,k) read by antidiagonals. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the antidiagonal d that contains the n‑th term.
       The number of terms up to diagonal d is (d-1)d/2. */
    size_t d = 2;
    size_t t;
    for (;; d++) {
        t = (d - 1) * d / 2;
        if (t >= (size_t)n)
            break;
    }

    /* The largest index needed in the 2‑D array is d-1. */
    size_t need_dim = d - 1;

    /* Static state to reuse computations between calls. */
    static size_t current_dim = 0;
    static seq_t *matrix = NULL;

    if (current_dim < need_dim) {
        /* Re‑allocate a larger square array and recompute it. */
        size_t new_dim = need_dim;
        size_t stride = new_dim + 1;               /* 1‑based indices */
        size_t new_size = stride * stride;        /* number of elements */

        seq_t *new_mat = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_mat)
            return 0; /* out of memory – caller will not expect this case */

        /* Set boundary values a(n,1)=1 and a(1,k)=1. */
        for (size_t i = 1; i <= new_dim; ++i)
            new_mat[i * stride + 1] = (seq_t)1;
        for (size_t j = 1; j <= new_dim; ++j)
            new_mat[1 * stride + j] = (seq_t)1;

        /* Fill the interior using the recurrence. */
        for (size_t i = 2; i <= new_dim; ++i) {
            for (size_t j = 2; j <= new_dim; ++j) {
                new_mat[i * stride + j] =
                    (seq_t)1
                  + new_mat[(i - 1) * stride + (j - 1)]
                  + new_mat[(i - 1) * stride + j]
                  + new_mat[i * stride + (j - 1)];
            }
        }

        /* Replace old table. */
        if (matrix)
            free(matrix);
        matrix = new_mat;
        current_dim = new_dim;
    }

    /* Map the nth index to coordinates (i,j). */
    size_t prev_T = (d - 2) * (d - 1) / 2;        /* number of terms before diagonal d */
    size_t offset = (size_t)n - prev_T;          /* 1 … d-1 */
    size_t i = offset;                           /* row index */
    size_t j = d - offset;                       /* column index */
    size_t stride = current_dim + 1;

    return matrix[i * stride + j];
}
