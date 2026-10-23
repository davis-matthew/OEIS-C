/*
 * A022176  Triangle of Gaussian binomial coefficients [ n,k ] for q = 12.
 * https://oeis.org/A022176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the Gaussian binomial coefficient * [ r , c ]_12 written
 *            row‑by‑row (row 0 has 1 element, * row 1 has 2, …). For a given
 *            index n: * * r = ⌊(√(8n+1) – 1)/2⌋ (row number) * c = n –
 *            r(r+1)/2 (column within the row) * * Recurrence used to build
 *            the triangle: * * T(0,0) = 1 * T(n,0) = 1 * T(n,...
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 23, u64 n <= 47, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* Static state: array of pointers to already computed rows.
 * rows[i] points to an array of length i+1 containing T(i,0)…T(i,i). */
static seq_t **rows = NULL;          /* NULL at start */
static idx_t   rows_capacity = 0;    /* capacity of 'rows' array */
static idx_t   rows_count   = 0;     /* number of rows that are computed */

/* Ensure that rows 0 … required_row are computed. */
static void
ensure_rows(idx_t required_row)
{
    /* Grow the rows array if necessary. */
    if (rows_count == 0 || rows_capacity == 0) {
        /* initial allocation */
        rows_capacity = 8;
        rows = (seq_t **)malloc(rows_capacity * sizeof(seq_t *));
        if (!rows)
            return;                     /* out of memory: caller will receive 0 */
    }
    while (rows_count <= required_row) {
        /* grow rows array if needed */
        if (rows_count >= rows_capacity) {
            size_t new_cap = rows_capacity * 2;
            seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
            if (!tmp)
                return;                 /* out of memory */
            rows      = tmp;
            rows_capacity = new_cap;
        }

        /* Compute new row i = rows_count. */
        idx_t i = rows_count;
        seq_t *row = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        if (!row)
            return;                     /* out of memory */
        row[0] = (seq_t)1;               /* first element is always 1 */

        if (i > 0) {
            const seq_t q = (seq_t)12;   /* base q of the Gaussian coefficients */
            seq_t pow_qk   = q;          /* 12^1 for k = 1 */
            /* k runs from 1 to i-1 (last element is 1). */
            for (idx_t k = 1; k < i; ++k) {
                /* T(i,k) = T(i-1,k-1) + 12^k * T(i-1,k) */
                row[k] = rows[i - 1][k - 1] + pow_qk * rows[i - 1][k];
                pow_qk *= q;              /* advance to 12^(k+1) */
            }
            row[i] = (seq_t)1;           /* last element of the row */
        }

        rows[rows_count] = row;
        rows_count++;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* Determine (r,c) from the flattened index n. */
    double d = sqrt((double)(8 * (double)n + 1));
    idx_t r = (idx_t)((d - 1) / 2);              /* row number */

    /* start = r*(r+1)/2  (using seq_t to avoid overflow); */
    seq_t start_seq = (seq_t)r * ((seq_t)r + 1) / 2;
    idx_t start = (idx_t)start_seq;              /* start index of the row */

    idx_t c = n - start;                         /* column within the row */

    /* Compute all rows up to r if not already done. */
    ensure_rows(r);
    if (r >= rows_count)    /* memory allocation failed */
        return (seq_t)0;     /* safe fallback */

    return rows[r][c];
}
