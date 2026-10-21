/*
 * A060137  Square array read by antidiagonals with
 *          T(n,k)=T(n,k-1)^2-n*T(n,k-1)+1 and T(n,0)=0.
 * https://oeis.org/A060137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n,k) = T(n,k-1)^2 - n*T(n,k-1) + 1, T(n,0) = 0 a(n) is the
 *            square array read by antidiagonals: a(n) = T(i,j) where i+j = s,
 *            i = n - s*(s+1)/2
 * Range:     i8 n <= 14, i16 n <= 20, i32 n <= 27, i64 n <= 35, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Lazy memoisation of rows of the array */
static seq_t **row = NULL;          /* pointer to array of rows */
static size_t *row_len = NULL;      /* number of elements computed in each row */
static size_t rows_cap = 0;         /* allocated capacity */

static void ensure_rows(size_t need)
{
    if (need < rows_cap) return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 1;
    while (new_cap <= need) new_cap *= 2;
    row = (seq_t **)realloc(row, new_cap * sizeof(seq_t *));
    row_len = (size_t *)realloc(row_len, new_cap * sizeof(size_t));
    rows_cap = new_cap;
}

static seq_t get_T(idx_t n, idx_t k)
{
    /* Ensure row exists */
    if ((size_t)n >= rows_cap) ensure_rows(n);
    if (!row[n]) {
        row[n] = (seq_t *)malloc(sizeof(seq_t));
        row_len[n] = 1;
        row[n][0] = 0;            /* T(n,0) = 0 */
    }
    /* Extend row to required column k */
    if ((size_t)k + 1 > row_len[n]) {
        size_t old_len = row_len[n];
        size_t new_len = (size_t)k + 1;
        row[n] = (seq_t *)realloc(row[n], new_len * sizeof(seq_t));
        for (size_t idx = old_len; idx < new_len; ++idx) {
            seq_t prev = row[n][idx - 1];
            row[n][idx] = prev * prev - (seq_t)n * prev + 1;
        }
        row_len[n] = new_len;
    }
    return row[n][k];
}

seq_t a(idx_t n)
{
    /* Find antidiagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while (s * (s + 1) / 2 <= n) ++s;
    --s;
    idx_t tri = s * (s + 1) / 2;
    idx_t offset = n - tri;
    idx_t i = offset;
    idx_t j = s - offset;
    return get_T(i, j);
}
