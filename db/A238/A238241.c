/*
 * A238241  Riordan array (1/(1-x-x^2)^2, x/(1-x-x^2)^2).
 * https://oeis.org/A238241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A238241: row‑major enumeration of the Riordan array
 *  (1/(1-x-x²)² , x/(1-x-x²)²) where
 *      T(0,0) = 1,
 *      T(n,k) = 0 if k<0 or k>n,
 *      for n≥0,
 *      T(n,k) = T(n-1,k-1) + 2*T(n-1,k) + T(n-2,k)
 *                - 2*T(n-3,k) - T(n-4,k).
 *  a(n) is the n‑th entry obtained by reading the rows left to right.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Memoisation of computed rows. */
    static seq_t **rows      = NULL; /* array of pointers to rows        */
    static size_t rows_cap  = 0;     /* capacity of rows[]               */
    static size_t row_cnt   = 0;     /* number of rows stored            */
    static idx_t  cum_terms = 0;     /* total number of terms stored     */

    /* Grow the triangle until we have at least n+1 terms. */
    while (cum_terms <= n) {
        size_t r   = row_cnt;        /* index of the next row to compute */
        size_t len = r + 1;          /* row length = r+1                 */

        /* Allocate or grow the rows array if necessary. */
        if (row_cnt == rows_cap) {
            size_t new_cap = rows_cap ? rows_cap * 2 : 4;
            seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
            if (!tmp) return 0;                  /* allocation failure */
            rows     = tmp;
            rows_cap = new_cap;
        }

        /* Allocate the new row. */
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row) return 0;                      /* allocation failure */

        /* Compute the entries of the new row. */
        for (size_t k = 0; k < len; ++k) {
            if (r == 0) {                /* base row */
                row[0] = 1;
                continue;
            }
            seq_t val = 0;

            /* T(n-1,k-1) */
            if (k > 0 && k - 1 < r)
                val += rows[r - 1][k - 1];

            /* 2*T(n-1,k) */
            if (k <= r - 1)
                val += rows[r - 1][k] * 2;

            /* T(n-2,k) */
            if (r >= 2 && k <= r - 2)
                val += rows[r - 2][k];

            /* -2*T(n-3,k) */
            if (r >= 3 && k <= r - 3)
                val -= rows[r - 3][k] * 2;

            /* -T(n-4,k) */
            if (r >= 4 && k <= r - 4)
                val -= rows[r - 4][k];

            row[k] = val;
        }

        /* Store the new row and update counters. */
        rows[r]      = row;
        cum_terms   += (idx_t)len;
        ++row_cnt;
    }

    /* Find the row that contains the requested term. */
    idx_t cum = 0;
    for (size_t r = 0; r < row_cnt; ++r) {
        size_t len = r + 1;
        if (n < cum + len) {
            idx_t offset = n - cum;
            return rows[r][offset];
        }
        cum += (idx_t)len;
    }

    return 0; /* unreachable, but keeps the compiler happy */
}
