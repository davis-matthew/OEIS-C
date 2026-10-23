/*
 * A186827  Riordan array (1-x, x(1-x)/(1+x)).
 * https://oeis.org/A186827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   Row‐wise read of the Riordan triangle T(n,k) = T(n‑1,k‑1) –
 *            T(n‑1,k) – T(n‑2,k‑1), with T(0,0)=1, T(1,0)=‑1, T(1,1)=1,
 *            T(2,0)=0, T(2,1)=‑3, T(2,2)=1. a(n) = T(r,c) where r(r+1)/2 ≤ n
 *            < (r+1)(r+2)/2 and c = n‑r(r+1)/2
 * Range:     i8 n <= 38, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* static storage for triangle rows */
static seq_t **rows      = NULL;  /* array of pointers to each row */
static idx_t rows_count  = 0;     /* number of rows currently stored   */
static size_t rows_cap   = 0;     /* current capacity of the rows array */

/* ------------------------------------------------------------------ */
/* Compute missing rows up to and including row r.                    */
/* ------------------------------------------------------------------ */
static void
ensure_rows_up_to(idx_t r)
{
    /* Grow the rows pointer array if necessary. */
    if (r + 1 > rows_cap) {
        size_t new_cap = rows_cap ? rows_cap << 1 : 1;
        while (new_cap < (size_t)(r + 1)) new_cap <<= 1;
        rows = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        rows_cap = new_cap;
    }

    /* Allocate and fill missing rows. */
    for (idx_t i = rows_count; i <= r; ++i) {
        size_t len = (size_t)i + 1;
        rows[i] = (seq_t *)malloc(len * sizeof(seq_t));
        if (!rows[i]) return;   /* out of memory – caller may check */

        if (i == 0) {
            rows[0][0] = (seq_t)1;
        } else if (i == 1) {
            rows[1][0] = (seq_t)-1;
            rows[1][1] = (seq_t)1;
        } else if (i == 2) {
            rows[2][0] = (seq_t)0;
            rows[2][1] = (seq_t)-3;
            rows[2][2] = (seq_t)1;
        } else {
            /* Use recurrence T(i,k) = T(i-1,k-1) – T(i-1,k) – T(i-2,k-1) */
            for (idx_t k = 0; k <= i; ++k) {
                seq_t term1 = (k >= 1) ? rows[i-1][k-1] : (seq_t)0;
                seq_t term2 = (k <= i-1) ? rows[i-1][k] : (seq_t)0;
                seq_t term3 = (k >= 1 && k <= i-1) ? rows[i-2][k-1] : (seq_t)0;
                rows[i][k] = term1 - term2 - term3;
            }
        }
    }
    rows_count = r + 1;
}

/* ------------------------------------------------------------------ */
/* Find row r and column c for the triangular index n.                */
/* ------------------------------------------------------------------ */
static void
row_col(idx_t n, idx_t *row, idx_t *col)
{
    /* Initial estimate using double square root. */
    double d = sqrt(8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    /* Correct any rounding error. */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    *row = r;
    *col = n - r * (r + 1) / 2;
}

/* ------------------------------------------------------------------ */
/* Public entry point – compute the n‑th term of A186827.             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    idx_t r, c;
    row_col(n, &r, &c);
    ensure_rows_up_to(r);
    return rows[r][c];
}
