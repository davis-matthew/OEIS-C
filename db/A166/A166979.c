/*
 * A166979  Triangle T(n,k) read by rows: T(n, k) = (m*n - m*k + 1)*T(n - 1, k
 *          - 1) + (9*k - 8)*(m*k - (m - 1))*T(n - 1, k) where m = 0.
 * https://oeis.org/A166979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 57, u64 n <= 139,
 *            u128 n <= 325, big any n
 * Helpers:   seq_alloc_array, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: T(n,k) = T(n-1,k-1) + (9*k-8)*T(n-1,k),  T(1,1)=1
 *           (read by rows, a(n) is the n‑th entry in this triangle)
 *
 * Sequence: A166979
 */

#include <stdlib.h>
#include "../utils/seq_one.h"
#include "../utils/seq_alloc_array.h"

/* Triangle stored as an array of pointers; row r has r entries (1‑based). */
static seq_t **triangle = NULL;
static idx_t max_computed = 0;   /* largest row already computed */

/* Return the n‑th element of the triangle (1‑based indexing). */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Offset of the first element of row r. */
    idx_t offset = ((r - 1) * r) / 2;
    idx_t col_idx = n - offset - 1;   /* 0‑based column index in row r */

    /* Allocate and compute missing rows up to r. */
    if (max_computed < r) {
        /* Allocate the array of row pointers to hold indices 1..r. */
        size_t new_cap = (size_t)(r + 1); /* index 0 is unused */
        if (triangle == NULL) {
            triangle = (seq_t **)(void *)malloc(new_cap * sizeof(seq_t *));
        } else {
            triangle = (seq_t **)(void *)realloc(triangle, new_cap * sizeof(seq_t *));
        }
        if (triangle == NULL)
            abort();    /* allocation failure */

        /* Compute missing rows. */
        for (idx_t row = max_computed + 1; row <= r; ++row) {
            seq_t *curr = seq_alloc_array((size_t)row);
            curr[0] = seq_one();        /* first element is always 1 */

            if (row > 1) {
                seq_t *prev = triangle[row - 1];
                /* Middle elements (if any). */
                for (idx_t j = 1; j + 1 < row; ++j) {
                    /* k = j+1 in 1‑based indexing. */
                    seq_t left  = prev[j - 1];
                    seq_t right = prev[j] * (seq_t)(9 * (j + 1) - 8);
                    curr[j] = left + right;
                }
                curr[row - 1] = seq_one();  /* last element is always 1 */
            }

            triangle[row] = curr;
        }
        max_computed = r;
    }

    /* Return the requested element. */
    return triangle[r][col_idx];
}
