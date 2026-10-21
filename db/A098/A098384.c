/*
 * A098384  Triangle read by rows of coefficients used to generate diagonals
 *          of ordered factorizations as displayed in A098348.
 * https://oeis.org/A098384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   a(n) is obtained by reading the triangle of coefficients T(n,k)
 *            row‑major, where T(0,0) = 1, T(1,0)=3, T(1,1)=2, and for n≥1,
 *            k=0…n T(n,k) = 2*k*T(n‑1,k‑1) + 3*(k+1)*T(n‑1,k) +
 *            (k+2)*T(n‑1,k+1) (with out‑of‑range indices treated as 0)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for computed rows of the triangle */
static seq_t **rows = NULL;      /* array of pointers to row arrays   */
static size_t *row_lengths = NULL; /* number of elements in each row   */
static size_t rows_count = 0;    /* number of rows currently stored   */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initialise the triangle with T(0,0) = 1 */
    if (rows_count == 0) {
        rows = (seq_t**)malloc(sizeof(seq_t*));
        row_lengths = (size_t*)malloc(sizeof(size_t));
        rows[0] = (seq_t*)malloc(sizeof(seq_t));
        rows[0][0] = (seq_t)1;     /* T(0,0) */
        row_lengths[0] = 1;        /* row 0 has 1 element */
        rows_count = 1;
    }

    /* compute rows until we have at least the row containing term n */
    /* the linear index where row r begins is r*(r+1)/2            */
    idx_t start_of_next_row = (idx_t)rows_count * (rows_count + 1) / 2; /* start of row rows_count */
    while (start_of_next_row <= n) {
        /* build next row */
        size_t prev_len = row_lengths[rows_count - 1];
        seq_t *prev_row = rows[rows_count - 1];
        size_t new_len = prev_len + 1;                     /* row has one more element */
        seq_t *new_row = (seq_t*)malloc(new_len * sizeof(seq_t));

        for (size_t k = 0; k < new_len; k++) {
            seq_t val = 0;

            if (k > 0) {
                val += prev_row[k - 1] * (seq_t)(2 * k);
            }
            if (k <= prev_len - 1) {
                val += prev_row[k] * (seq_t)(3 * (k + 1));
            }
            if (k + 1 <= prev_len - 1) {
                val += prev_row[k + 1] * (seq_t)(k + 2);
            }
            new_row[k] = val;
        }

        /* append new row to storage */
        rows = (seq_t**)realloc(rows, (rows_count + 1) * sizeof(seq_t*));
        row_lengths = (size_t*)realloc(row_lengths, (rows_count + 1) * sizeof(size_t));
        rows[rows_count] = new_row;
        row_lengths[rows_count] = new_len;
        rows_count++;

        /* update start of next row for the loop test */
        start_of_next_row = (idx_t)rows_count * (rows_count + 1) / 2;
    }

    /* after the loop, the required term lies in row (rows_count-1) */
    size_t r = rows_count - 1;
    idx_t row_start = (idx_t)r * (r + 1) / 2; /* first index of this row */
    idx_t k = n - row_start;                  /* column inside the row */

    return rows[r][(size_t)k];
}
