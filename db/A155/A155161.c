/*
 * A155161  A Fibonacci convolution triangle: Riordan array (1, x/(1 - x -
 *          x^2)). Triangle T(n,k), 0 <= k <= n, read by rows.
 * https://oeis.org/A155161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 140, u32 n <= 443, u64 n <= 1555,
 *            u128 n <= 5599, big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(n+2,k+1)=T(n+1,k+1)+T(n+1,k)+T(n,k+1),
 * with T(0,0)=1, T(n,0)=0 for n>0, T(n,k)=0 when k>n.
 * The sequence a(n) is the row‑wise flattening of the triangle.
 * a(0)=1, a(1)=0, a(2)=1, …  (OEIS A155161). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"

seq_t a(idx_t n)
{
    /* Determine the triangle row r and column c corresponding
       to the n‑th element in the row‑major ordering. */
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n)
        ++r;

    idx_t base = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - base;                     /* 0 ≤ c ≤ r */

    /* Cache of computed rows. */
    static seq_t **row_ptrs   = NULL;
    static size_t row_ptrs_cap = 0;
    static idx_t max_row = 0;

    if (row_ptrs == NULL) {                 /* first call – initialise */
        row_ptrs_cap = 2;                   /* enough for rows 0 and 1 */
        row_ptrs = (seq_t **)malloc(row_ptrs_cap * sizeof(seq_t *));
        if (!row_ptrs)
            return 0;                       /* allocation failure */

        /* row 0: [1] */
        seq_t *row0 = seq_alloc_new_array(1);
        if (!row0)
            return 0;
        row0[0] = 1;
        row_ptrs[0] = row0;

        /* row 1: [0,1] */
        seq_t *row1 = seq_alloc_new_array(2);
        if (!row1)
            return 0;
        row1[0] = 0;
        row1[1] = 1;
        row_ptrs[1] = row1;
        max_row = 1;
    }

    /* Extend cache to include row r if necessary. */
    if (r > max_row) {
        if ((size_t)r >= row_ptrs_cap) {
            size_t new_cap = row_ptrs_cap;
            while ((size_t)r >= new_cap)
                new_cap <<= 1;
            seq_t **tmp = (seq_t **)realloc(row_ptrs, new_cap * sizeof(seq_t *));
            if (!tmp)
                return 0;
            row_ptrs = tmp;
            row_ptrs_cap = new_cap;
        }

        for (idx_t i = max_row + 1; i <= r; ++i) {
            seq_t *row = seq_alloc_new_array(i + 1);
            if (!row)
                return 0;
            row[0] = 0;                     /* T(i,0)=0 for i>0 */

            if (i == 1) {                   /* already initialised above */
                row[1] = 1;
            } else {
                seq_t *prev  = row_ptrs[i - 1];
                seq_t *prev2 = row_ptrs[i - 2];
                for (idx_t k = 1; k <= i; ++k) {
                    seq_t val = 0;
                    if (k <= i - 1)
                        val += prev[k];
                    val += prev[k - 1];
                    if (k <= i - 2)
                        val += prev2[k];
                    row[k] = val;
                }
            }
            row_ptrs[i] = row;
        }
        max_row = r;
    }

    return row_ptrs[r][c];
}
