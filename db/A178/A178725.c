/*
 * A178725  Irregular triangle read by rows: row n gives coefficients in
 *          expansion of Product_{k=1..n} (1 + x^(4*k - 1)) for n >= 0.
 * https://oeis.org/A178725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 209, u16 n <= 209, u32 n <= 209, u64 n <= 209,
 *            u128 n <= 209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A178725: a(n) is the coefficient of x^e in
   the expansion of ∏_{k=1..r} (1 + x^(4k-1)),
   where n is the linear index obtained by concatenating
   all rows (row r has length 2r^2 + r + 1).
   Recurrence for each row:  coeff_r[e] = coeff_{r-1}[e] + coeff_{r-1}[e-w_r],
   where w_r = 4r-1 and coeff_0[0] = 1. */

/* The function is memoised; it works for any order of calls. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static storage for rows of the triangle. */
    static seq_t **rows = NULL;      /* rows[r] points to array of length row_size[r] */
    static size_t *row_size = NULL;  /* number of coefficients in each row (max_exp+1) */
    static size_t *row_start = NULL; /* starting index of each row in the flat sequence */
    static size_t rows_computed = 0;
    static size_t cap = 0;

    /* Initialise for row 0. */
    if (!rows_computed) {
        cap = 4;  /* initial capacity for 4 rows */
        rows = (seq_t**)malloc(sizeof(seq_t*) * cap);
        row_size = (size_t*)malloc(sizeof(size_t) * cap);
        row_start = (size_t*)malloc(sizeof(size_t) * (cap + 1));

        /* Row 0: one coefficient 1. */
        rows[0] = (seq_t*)calloc(1, sizeof(seq_t));
        rows[0][0] = (seq_t)1;
        row_size[0] = 1;

        row_start[0] = 0;
        row_start[1] = 1;   /* start of next row (row 1) */

        rows_computed = 1;
    }

    /* Expand rows until we reach the desired global index. */
    while (row_start[rows_computed] <= (size_t)n) {
        size_t r = rows_computed;          /* index of the new row to compute */
        size_t prev = r - 1;
        size_t weight = 4 * r - 1;         /* exponent added by the new factor */

        size_t prev_size = row_size[prev];
        size_t new_size = prev_size + weight;  /* new max exponent + 1 */

        /* Grow storage arrays if necessary. */
        if (r >= cap) {
            size_t new_cap = cap * 2;
            rows = (seq_t**)realloc(rows, sizeof(seq_t*) * new_cap);
            row_size = (size_t*)realloc(row_size, sizeof(size_t) * new_cap);
            row_start = (size_t*)realloc(row_start, sizeof(size_t) * (new_cap + 1));
            cap = new_cap;
        }

        /* Allocate and initialise new row array. */
        rows[r] = (seq_t*)calloc(new_size, sizeof(seq_t));

        /* Copy previous row coefficients and add shifted coefficients. */
        seq_t *prev_row = rows[prev];
        seq_t *cur_row = rows[r];
        for (size_t i = 0; i < prev_size; ++i) {
            seq_t val = prev_row[i];
            if (!val) continue;           /* skip zeros for speed */
            cur_row[i] += val;            /* coefficient without taking new factor */
            cur_row[i + weight] += val;   /* coefficient with new factor */
        }

        row_size[r] = new_size;
        row_start[r + 1] = row_start[r] + row_size[r];
        ++rows_computed;
    }

    /* Locate the row that contains the requested index. */
    size_t r = 0;
    while (r + 1 < rows_computed && row_start[r + 1] <= (size_t)n)
        ++r;

    size_t pos = (size_t)n - row_start[r];

    /* If the position exceeds the computed row length, the coefficient is 0. */
    if (pos >= row_size[r])
        return (seq_t)0;

    return rows[r][pos];
}
