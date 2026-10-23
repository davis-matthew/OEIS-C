/*
 * A177978  Triangle T(n,k) read by rows: A051731(n,k) - A051731(n-1,k).
 * https://oeis.org/A177978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 93, i16 n <= 93, i32 n <= 93, i64 n <= 93, i128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A177978
 * Sequence a(n) is the triangle T(n,k) read by rows:
 *   T(n,1) = 1 if n==1 else 0
 *   For k>1:  T(n,k) = (∑_{i=1}^{k-1} T(n-i,k-1)) - (∑_{i=1}^{k-1} T(n-i,k))
 * The term a(n) = T(r, k) where r is the row containing n (r(r+1)/2 ≥ n)
 * and k = n - r(r-1)/2.
 *
 * The implementation builds the triangle on demand and caches it.
 * It works with any seq_t/idx_t types defined by the build system.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Triangular data cache */
    static seq_t *T           = NULL;   /* flat array of all triangle entries */
    static idx_t *row_start   = NULL;   /* start index of each row in T (1‑based) */
    static size_t  T_cap      = 0;      /* allocated capacity of T        */
    static size_t  row_cap    = 0;      /* allocated capacity of row_start */
    static idx_t   rows       = 0;      /* number of rows already built (≤ rows) */

    /* Find the row r that contains term n */
    idx_t r = 1;
    while ( (idx_t)r * (r + 1) / 2 < n ) {
        ++r;
    }

    /* Build rows until we have at least r rows */
    while (rows < r) {
        ++rows;                       /* next row index (1‑based) */

        /* Ensure storage for row_start */
        if ((size_t)(rows + 1) > row_cap) {
            size_t new_cap = row_cap ? row_cap << 1 : 8;
            idx_t *new_ptr = (idx_t*)realloc(row_start, new_cap * sizeof(idx_t));
            row_start = new_ptr;
            row_cap   = new_cap;
        }

        /* Start index of the new row in T */
        if (rows == 1)
            row_start[1] = 0;
        else
            row_start[rows] = T_cap;   /* T_cap is number of elements already stored */

        /* Allocate space for the new row */
        size_t row_len = rows;         /* row has exactly rows entries */
        if (T_cap + row_len > T_cap) {
            size_t new_cap = T_cap ? T_cap << 1 : row_len << 1;
            while (new_cap < T_cap + row_len)
                new_cap <<= 1;
            seq_t *new_ptr = (seq_t*)realloc(T, new_cap * sizeof(seq_t));
            T = new_ptr;
            T_cap = new_cap;
        }

        /* Compute triangle entries for this row */
        seq_t *row = T + row_start[rows];
        for (size_t k = 1; k <= row_len; ++k) {
            if (k == 1) {               /* first column */
                row[0] = (rows == 1) ? 1 : 0;
            } else {
                seq_t sum1 = 0, sum2 = 0;
                for (size_t i = 1; i < k; ++i) {
                    idx_t prior_row = rows - (idx_t)i;
                    /* T(prior_row, k-1) */
                    if ((idx_t)(k-1) <= prior_row) {
                        sum1 += T[row_start[prior_row] + (k-1) - 1];
                    }
                    /* T(prior_row, k) */
                    if ((idx_t)k <= prior_row) {
                        sum2 += T[row_start[prior_row] + k - 1];
                    }
                }
                row[k-1] = sum1 - sum2;
            }
        }

        /* Update number of stored elements */
        T_cap += row_len;
    }

    /* Retrieve the requested term */
    idx_t start = row_start[r];
    idx_t k     = n - (r-1)*r/2;      /* k is 1‑based column in row r */
    return T[start + k - 1];
}
