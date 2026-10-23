/*
 * A108083  Triangle, read by rows, where T(0,0) = 1, T(n,k) =
 *          (-1)^n*(2n+1)*T(n-1,k) - T(n-1,k-1).
 * https://oeis.org/A108083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(0,0) = 1, T(n,k) = (-1)^n*(2n+1)*T(n-1,k) - T(n-1,k-1) a(n) =
 *            T( r , k ) where n = r(r+1)/2 + k , 0 ≤ k ≤ r
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/*  Static storage for already‑computed rows of the triangle.
    rows[0] contains {1}.  rows[1] contains the second row, etc.
    row_len[i] = i+1 (length of row i).  max_row is the highest
    indexed row that has been computed.  The arrays are enlarged
    lazily on demand.  All calculations use only seq_t operations. */
static seq_t **rows = NULL;          /* array of pointers to rows  */
static idx_t *row_len = NULL;        /* lengths of the rows       */
static idx_t max_row = 0;            /* greatest computed row ID   */

/*  Ensure the storage arrays are large enough for rows up to 'need'.
    The function expands rows[] and row_len[] using realloc. */
static void ensure_rows_capacity(idx_t need)
{
    if (!rows || need >= max_row + 1) {
        size_t new_cap = (rows ? 2 * (size_t)need : 16);
        if (new_cap < 16) new_cap = 16;
        rows = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        row_len = (idx_t *)realloc(row_len, new_cap * sizeof(idx_t));
        max_row = new_cap - 1;         /* max index currently allocated */
    }
}

/*  Compute all rows up to 'target_row' inclusive.
    Each new row is created from the previous one using the
    recurrence.  The function works even if target_row
    has already been computed. */
static void compute_rows_up_to(idx_t target_row)
{
    if (!rows) {                      /* first call: create row 0 */
        ensure_rows_capacity(1);
        rows[0] = (seq_t *)malloc(sizeof(seq_t));
        rows[0][0] = 1;               /* T(0,0)=1 */
        row_len[0] = 1;
        max_row = 0;
    }

    while (max_row < target_row) {
        idx_t prev = max_row;
        idx_t curr = prev + 1;
        idx_t len_prev = row_len[prev];
        idx_t len_curr = len_prev + 1;

        ensure_rows_capacity(curr + 1);
        rows[curr] = (seq_t *)malloc(len_curr * sizeof(seq_t));
        row_len[curr] = len_curr;

        /* (-1)^curr */
        seq_t sgn = ( (curr & 1U) ? (seq_t)-1 : (seq_t)1 );

        /* compute current row */
        for (idx_t k = 0; k < len_curr; ++k) {
            seq_t value = 0;
            if (k < len_prev) {
                value = sgn * (seq_t)(2 * curr + 1) * rows[prev][k];
            }
            if (k > 0) {
                value -= rows[prev][k - 1];
            }
            rows[curr][k] = value;
        }
        max_row = curr;
    }
}

/*  Find the row r such that n lies in row r (0‑based). */
static idx_t row_of(idx_t n)
{
    idx_t r = 0;
    while ( (r + 1) * (r + 2) / 2 <= n ) {
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Determine the row and column of the requested index. */
    idx_t row = row_of(n);
    idx_t start_of_row = row * (row + 1) / 2;
    idx_t col = n - start_of_row;

    /* Ensure all rows up to 'row' are computed. */
    compute_rows_up_to(row);

    /* Return the value T(row, col). */
    return rows[row][col];
}
