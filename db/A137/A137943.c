/*
 * A137943  This sequence needs a proper definition.
 * https://oeis.org/A137943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, uned, sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 28, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients of the Sheffer sequence defined by
   (OEIS A137943):
       T_0(x) = -1,
       T_1(x) = -x,
       T_n(x) = 3*n*(n-1)*T_{n-2}(x) + 2*n*(n-1)*(n-2)*T_{n-3}(x) - x^n   (n ≥ 2).
   a(n) is obtained by flattening all rows of T_k(x) (k = 0,1,2,…) left‑to‑right.
   The first few rows are:
       -1
       0, -1
      -6,  0, -1
     -12, -18, 0, -1
     -216, -48, -36, 0, -1
   etc. */
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/mul_small.h"

static seq_t **rows   = NULL;  /* pointers to each polynomial row (ascending coeffs) */
static size_t rows_count = 0;  /* number of rows currently stored */
static size_t rows_alloc = 0;  /* allocated capacity for rows array */

/* Generate the k‑th row (0‑based) of the Sheffer sequence.
   The row contains k+1 coefficients in ascending powers of x. */
static void generate_row(size_t k)
{
    seq_t *row = (seq_t *)malloc((k + 1) * sizeof(seq_t));
    if (!row) return;              /* allocation failed – caller will handle */
    for (size_t i = 0; i <= k; ++i)
        row[i] = (seq_t)0;

    if (k == 0) {
        row[0] = (seq_t)-1;
    } else if (k == 1) {
        row[0] = (seq_t)0;
        row[1] = (seq_t)-1;
    } else {
        unsigned int scalar1 = 3u * (unsigned int)k * (unsigned int)(k - 1);
        unsigned int scalar2 = 2u * (unsigned int)k * (unsigned int)(k - 1) *
                               (unsigned int)(k - 2);

        /* Contribution from T_{k-2}(x) */
        for (size_t i = 0; i <= k - 2; ++i)
            row[i] += mul_small(rows[k - 2][i], scalar1);

        /* Contribution from T_{k-3}(x), if applicable */
        if (k >= 3) {
            for (size_t i = 0; i <= k - 3; ++i)
                row[i] += mul_small(rows[k - 3][i], scalar2);
        }

        /* The term -x^k */
        row[k] = (seq_t)-1;
    }

    /* Ensure there is enough space in the rows array. */
    if (rows_count == rows_alloc) {
        size_t new_alloc = rows_alloc == 0 ? 16 : rows_alloc * 2;
        seq_t **new_rows = (seq_t **)realloc(rows, new_alloc * sizeof(seq_t *));
        if (!new_rows) return;        /* allocation failed – caller will handle */
        rows = new_rows;
        rows_alloc = new_alloc;
    }
    rows[rows_count++] = row;
}

/* Return the n‑th term (1‑based) of A137943. */
seq_t a(idx_t n)
{
    /* Determine the row index r (0‑based) such that the first r+1 rows
       contain at least n elements. */
    size_t r = 0;
    while ((r + 1) * (r + 2) / 2 < n) ++r;
    size_t row_idx = r;                    /* 0‑based index of the row */
    size_t prev_tri = r * (r + 1) / 2;     /* total elements before this row */
    size_t col_idx = (size_t)(n - prev_tri - 1);  /* column within the row */

    /* Generate all rows up to the requested one. */
    while (rows_count <= row_idx)
        generate_row(rows_count);

    return rows[row_idx][col_idx];
}
