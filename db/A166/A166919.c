/*
 * A166919  Irregular triangle of coefficients of Product_{j=1..n} (x^j - x -
 *          1), read by rows.
 * https://oeis.org/A166919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 21, i16 n <= 84, i32 n <= 364, i64 n <= 1540,
 *            i128 n <= 4960, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A166919 – Irregular triangle of coefficients of
 *  Product_{j=1..n} (x^j - x - 1), read by rows.
 *
 *  Recurrence for the triangle entries T(n,k):
 *       T(0,0) = 1
 *       T(n,k) = -(n) * T(n-1,k)   - T(n-1,k-1)   + T(n-1,k-n)
 *   (indices outside the valid range contribute 0).
 *
 *  The sequence a(n) is the row‑by‑row read of the triangle:
 *       a(0)=1, a(1)=-1, a(2)=2, a(3)=1, a(4)=-1, …
 *  The implementation builds the triangle rows on demand and
 *  caches them so that calls may be in any order.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Static tables for on‑demand row construction */
static seq_t **poly_rows   = NULL;   /* poly_rows[i] points to row i coefficients */
static idx_t *row_start    = NULL;   /* starting linear index of row i in a(n) */
static size_t *row_len     = NULL;   /* number of entries in row i */
static size_t rows_alloc   = 0;      /* allocated number of rows */
static size_t rows_used    = 0;      /* actual number of rows built */

/* Ensure that poly_rows, row_start, and row_len have space for at least r+1 rows */
static void ensure_rows_up_to(size_t r)
{
    if (r < rows_used) return;

    /* Allocate or grow the tables */
    if (poly_rows == NULL) {
        rows_alloc   = r + 1;
        poly_rows    = (seq_t **)malloc(rows_alloc * sizeof(seq_t *));
        row_start    = (idx_t   *)malloc(rows_alloc * sizeof(idx_t));
        row_len      = (size_t *)malloc(rows_alloc * sizeof(size_t));
    } else if (rows_alloc <= r) {
        size_t new_alloc = rows_alloc * 2;
        if (new_alloc <= r) new_alloc = r + 1;
        poly_rows = (seq_t **)realloc(poly_rows, new_alloc * sizeof(seq_t *));
        row_start = (idx_t   *)realloc(row_start, new_alloc * sizeof(idx_t));
        row_len   = (size_t *)realloc(row_len,   new_alloc * sizeof(size_t));
        rows_alloc = new_alloc;
    }

    /* Build rows sequentially from the first missing row up to r */
    for (size_t i = rows_used; i <= r; ++i) {
        if (i == 0) {                            /* row 0: coefficient 1 */
            poly_rows[0] = (seq_t *)malloc(sizeof(seq_t));
            poly_rows[0][0] = (seq_t)1;
            row_len[0]   = 1;
            row_start[0] = (idx_t)0;
            row_start[1] = row_start[0] + row_len[0];
        } else {
            size_t prev_len = row_len[i-1];
            size_t cur_len  = prev_len + i;          /* maximal length before trimming */
            seq_t *cur = (seq_t *)malloc(cur_len * sizeof(seq_t));

            for (size_t k = 0; k < cur_len; ++k) {
                seq_t a_k   = (k < prev_len)            ? poly_rows[i-1][k] : 0;
                seq_t a_k1  = (k >= 1 && (k-1) < prev_len) ? poly_rows[i-1][k-1] : 0;
                seq_t a_k_n = (k >= i && (k-i) < prev_len)  ? poly_rows[i-1][k-i] : 0;
                cur[k] = -( (seq_t)i * a_k ) - a_k1 + a_k_n;
            }

            /* Trim trailing zeros, keeping at least one coefficient */
            size_t last = cur_len;
            while (last > 0 && cur[last-1] == 0) --last;
            if (last == 0) last = 1;
            cur = (seq_t *)realloc(cur, last * sizeof(seq_t));

            poly_rows[i] = cur;
            row_len[i]   = last;
            row_start[i] = row_start[i-1] + row_len[i-1];
            row_start[i+1] = row_start[i] + row_len[i];
        }
    }
    rows_used = r + 1;
}

/* Find the row index r such that a(r,*) contains position n */
static idx_t find_row(idx_t n)
{
    if (rows_used == 0) ensure_rows_up_to(0);
    idx_t r = 0;
    while (true) {
        if ((size_t)(r + 1) >= rows_used)  /* need to build next row */
            ensure_rows_up_to((size_t)r + 1);

        if (n < row_start[r+1]) return r;
        ++r;
    }
}

/* Main entry point: returns the nth term of A166919 */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t pos = n - row_start[r];
    return poly_rows[r][pos];
}
