/*
 * A132845  Triangle, read by rows, where row n equals row n of matrix power
 *          A132844^n for n>=0, where triangle A132844 is defined by:
 *          A132844(n,k) = T( [(n+k)/2], k) for n>=k>=0.
 * https://oeis.org/A132845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n_row, n_k) where T is defined by: T(n,k) = [A(n,k)]^n
 *            (n,k) and A(n,k) = T(floor((n+k)/2), k) for n >= k >= 0. The
 *            triangle T is read by rows: a(0) = T(0,0), a(1) = T(1,0), a(2) =
 *            T(1,1), a(3) = T(2,0), a(4) = T(2,1), a(5) = T(2,2), ... The
 *            implementation computes the triangle increment...
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   global_index_to_rc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/global_index_to_rc.h"

static seq_t **T_triangle = NULL;   /* triangular array of T values */
static idx_t  triangle_rows = 0;    /* number of rows currently computed (0 means none) */

/* Ensure that rows 0..max_row are computed in T_triangle */
static void ensure_rows(idx_t max_row) {
    if (max_row < triangle_rows)
        return;    /* already done */

    /* Extend the array of row pointers if necessary */
    if (max_row + 1 > triangle_rows) {
        T_triangle = (seq_t **)realloc(T_triangle, (max_row + 1) * sizeof(seq_t *));
        for (idx_t i = triangle_rows; i <= max_row; ++i)
            T_triangle[i] = NULL;
    }

    /* Compute new rows sequentially */
    for (idx_t n = triangle_rows; n <= max_row; ++n) {
        /* Allocate space for the new row (n+1 elements) */
        seq_t *row = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        for (idx_t j = 0; j <= n; ++j)
            row[j] = 0;          /* initialize all to zero */

        /* DP arrays for computing (A^n)(n, *) */
        seq_t *cur  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        seq_t *nxt  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        for (idx_t j = 0; j <= n; ++j) {
            cur[j] = 0;
            nxt[j] = 0;
        }
        cur[n] = 1;             /* start at state n with 0 steps */

        /* Perform n steps of the Markov chain defined by A */
        for (idx_t step = 1; step <= n; ++step) {
            for (idx_t i = 0; i <= n; ++i) {
                /* skip if current state has zero weight */
                if (cur[i] == 0)
                    continue;
                for (idx_t j = 0; j <= i; ++j) {
                    seq_t Aij;
                    if (j == i)                 /* diagonal entry */
                        Aij = 1;
                    else {
                        idx_t idx = (i + j) / 2;   /* floor((i+j)/2) */
                        Aij = T_triangle[idx][j];
                    }
                    if (Aij == 0)                /* skip zero weights */
                        continue;
                    seq_t prod = cur[i] * Aij;    /* multiplication in seq_t */
                    nxt[j] = nxt[j] + prod;      /* addition in seq_t */
                }
            }
            /* swap cur and nxt, and clear nxt for next iteration */
            seq_t *tmp = cur;
            cur = nxt;
            nxt = tmp;
            for (idx_t j = 0; j <= n; ++j)
                nxt[j] = 0;
        }

        /* After n steps, cur holds T(n,j) for j=0..n */
        for (idx_t k = 0; k <= n; ++k)
            row[k] = cur[k];

        /* Clean up temporary arrays */
        free(cur);
        free(nxt);

        /* Store computed row */
        T_triangle[n] = row;
    }

    triangle_rows = max_row + 1;
}

/* Public function required by the problem statement */
seq_t a(idx_t n) {
    idx_t row, col;
    global_index_to_rc(n, &row, &col);
    ensure_rows(row);
    return T_triangle[row][col];
}
