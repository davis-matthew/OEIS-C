/*
 * A063265  Septinomial (also called heptanomial) coefficient array.
 * https://oeis.org/A063265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 189, u32 n <= 515, u64 n <= 1885,
 *            u128 n <= 1975, big any n
 * Helpers:   offset_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A063265 – Septinomial (heptanomial) coefficient array.
 * The triangle is defined recursively by:
 *   a(0,0) = 1
 *   a(n,k) = sum_{j=0}^{6} a(n-1, k-j)   (missing indices treated as 0)
 * The one‑dimensional sequence a(n) is obtained by listing the triangle
 * row by row (row 0 then row 1 then …).  This function returns that
 * flattened value.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/offset_of.h"

/* The types seq_t (value) and idx_t (index) are supplied by the build. */
static seq_t **rows = NULL;        /* array of pointers to rows */
static size_t *row_len = NULL;     /* length of each row in terms of elements */
static size_t rows_count = 0;      /* number of rows currently allocated */

/* Ensure that rows[0 .. r] have been generated.  New rows are produced
 * by convolution of the previous row with a 7‑term kernel.  Each row
 * has length 6*r + 1. */
static void ensure_rows_up_to(idx_t r)
{
    while ((idx_t)rows_count <= r) {
        /* Row to compute */
        idx_t new_r     = (idx_t)rows_count;
        idx_t len_prev  = (new_r == 0) ? 0 : row_len[new_r - 1];
        idx_t len_cur   = 6 * new_r + 1;         /* number of entries in this row */

        /* Allocate storage for the new row */
        seq_t *cur = (seq_t *)malloc(len_cur * sizeof(seq_t));
        if (!cur) return;                       /* allocation failed – abort */

        /* Zero initialise */
        for (idx_t j = 0; j < len_cur; ++j) cur[j] = 0;

        if (new_r == 0) {
            /* Base row: a(0,0)=1 */
            cur[0] = 1;
        } else {
            seq_t *prev = rows[new_r - 1];
            /* Compute each entry by summing the previous row entries
             * up to 6 positions back (kernel width 7). */
            for (idx_t k = 0; k < len_cur; ++k) {
                seq_t sum = 0;
                for (idx_t j = 0; j <= 6; ++j) {
                    if (k >= j && (k - j) < len_prev)
                        sum += prev[k - j];
                }
                cur[k] = sum;
            }
        }

        /* Grow the arrays that store rows and their lengths */
        if (rows_count == 0) {
            rows = (seq_t **)malloc(sizeof(seq_t *));
            row_len = (size_t *)malloc(sizeof(size_t));
        } else {
            rows = (seq_t **)realloc(rows, (rows_count + 1) * sizeof(seq_t *));
            row_len = (size_t *)realloc(row_len, (rows_count + 1) * sizeof(size_t));
        }

        rows[rows_count] = cur;
        row_len[rows_count] = (size_t)len_cur;
        ++rows_count;
    }
}

/* Return the n‑th element of the flattened septinomial triangle. */
seq_t a(idx_t n)
{
    /* Find the row r such that the index n lies in that row. */
    idx_t r = 0;
    while (true) {
        idx_t next_off = offset_of(r + 1);
        if (n < next_off)
            break;
        ++r;
    }
    /* Position within the row */
    idx_t k = n - offset_of(r);

    /* Ensure rows up to r are available. */
    ensure_rows_up_to(r);

    return rows[r][k];
}
