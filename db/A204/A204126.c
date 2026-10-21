/*
 * A204126  Array: row n shows the coefficients of the characteristic
 *          polynomial of the n-th principal submatrix of f(i,j)=(i if i=j and
 *          1 otherwise) (A204125).
 * https://oeis.org/A204126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 37, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204126: coefficients of the characteristic polynomial
 * det(f(i,j)-λI) for the matrix with f(i,j) = (i if i=j else 1).
 * The coefficients are produced iteratively using signed Stirling
 * numbers of the first kind.  Each row n (n ≥ 1) has n+1 entries
 * written from the constant term up to the leading λⁿ term.  The
 * sequence is the flattened array of all rows.  The implementation
 * uses O(r²) time and O(r²) memory, where r is the largest row
 * required (≈√(2·index)).  For indices ≥ 1 it returns the exact
 * coefficient as a seq_t value. */

#include <stdlib.h>

static seq_t **row_cache = NULL;       /* arrays of coefficients for each row */
static size_t   row_cache_cap = 0;      /* capacity of row_cache */
static size_t   max_row_cached = 0;     /* highest row index computed */

/* Current Stirling numbers of the first kind for the last computed row
 * S_prev[k] = s(current_n, k).  The array length is current_n+1. */
static seq_t *S_prev = NULL;
static idx_t  current_n = 0;            /* last n for which S_prev is valid */

/* Ensure that rows up to 'need_row' are computed and cached.  All
 * rows are stored in row_cache[1 .. max_row_cached]. */
static void ensure_rows(idx_t need_row)
{
    if (need_row <= max_row_cached) return;

    /* Grow the cache of row pointers if necessary */
    if (need_row >= row_cache_cap) {
        size_t new_cap = row_cache_cap ? row_cache_cap * 2 : 16;
        while (new_cap <= need_row) new_cap *= 2;
        seq_t **tmp = (seq_t**)realloc(row_cache, new_cap * sizeof(seq_t*));
        row_cache = tmp;
        for (size_t i = row_cache_cap; i < new_cap; ++i) row_cache[i] = NULL;
        row_cache_cap = new_cap;
    }

    /* Initialise Stirling numbers for n = 0 */
    if (S_prev == NULL) {
        S_prev = (seq_t*)malloc(sizeof(seq_t));
        S_prev[0] = (seq_t)1;
        current_n = 0;
    }

    /* Build rows incrementally until desired row is reached */
    for (idx_t n = current_n + 1; n <= need_row; ++n) {
        /* Allocate new Stirling row of length n+1 */
        idx_t new_len = n + 1;
        seq_t *S_cur = (seq_t*)malloc(new_len * sizeof(seq_t));

        /* Recurrence: s(n,0)=0; s(n,n)=1 */
        S_cur[0] = (seq_t)0;
        for (idx_t k = 1; k < n; ++k) {
            seq_t term1 = S_prev[k - 1];
            seq_t term2 = S_prev[k];
            seq_t prod  = (seq_t)( (seq_t)(n - 1) * term2 );
            S_cur[k] = term1 - prod;
        }
        S_cur[n] = (seq_t)1;

        /* Compute coefficients of the characteristic polynomial for this row.
         * D_m = a_m - (m+1) * a_{m+1},  a_m = (-1)^n * s(n,m),  a_{n+1}=0. */
        seq_t *coeff = (seq_t*)malloc(new_len * sizeof(seq_t));
        int sign = (n & 1) ? -1 : 1;
        seq_t *a = (seq_t*)malloc(new_len * sizeof(seq_t));
        for (idx_t m = 0; m <= n; ++m)
            a[m] = (sign == 1) ? S_cur[m] : -S_cur[m];

        for (idx_t m = 0; m < n; ++m)
            coeff[m] = a[m] - (seq_t)( (seq_t)(m + 1) * a[m + 1] );
        coeff[n] = a[n];

        /* Store the row in the cache */
        row_cache[n] = coeff;

        /* Clean up for next iteration */
        free(a);
        free(S_prev);
        S_prev = S_cur;
    }

    current_n = need_row;
    max_row_cached = need_row;
}

/* Map a global sequence index to its row and position within that row. */
static idx_t row_of(idx_t idx)
{
    /* Find smallest r such that r*(r+3)/2 >= idx */
    idx_t r = 1;
    while ( (r*(r+3)/2) < idx ) ++r;
    return r;
}

static idx_t pos_in_row(idx_t idx, idx_t r)
{
    idx_t prev_T = (r-1)*(r+2)/2;
    return idx - prev_T - 1;
}

/* Main sequence function requested by the build */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    idx_t r = row_of(n);
    ensure_rows(r);

    idx_t pos = pos_in_row(n, r);
    return row_cache[r][pos];
}
