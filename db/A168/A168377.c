/*
 * A168377  Riordan array (1/(1 + x), x*c(x)), where c(x) is the o.g.f. of
 *          Catalan numbers A000108.
 * https://oeis.org/A168377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the flattened list of the Riordan triangle * T(n,k)
 *            defined by * T(n,0) = (-1)^n * T(n,1) = sum_{j=0}^{n-1} T(n-1,j)
 *            * T(n,k) = T(n,k-1) - T(n-1,k-2) for 2 <= k <= n * The entry
 *            a(n) is T(r,k) where r is the unique row with * r(r+1)/2 <= n <
 *            (r+1)(r+2)/2 and k = n - r(r+1)/2
 * Range:     i8 n <= 36, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   offset_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/offset_row.h"

static seq_t **rows = NULL;       /* array of rows, each row is a seq_t*  */
static seq_t *row_sums = NULL;    /* sum of each row */
static size_t rows_cnt = 0;       /* number of rows built              */
static size_t rows_cap = 0;       /* capacity of rows array             */
static size_t sums_cap = 0;       /* capacity of row_sums array         */

static void ensure_rows_cap(size_t needed)
{
    if (needed <= rows_cap)
        return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t **new_rows = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
    rows = new_rows;
    rows_cap = new_cap;
}

static void ensure_sums_cap(size_t needed)
{
    if (needed <= sums_cap)
        return;
    size_t new_cap = sums_cap ? sums_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_sums = (seq_t *)realloc(row_sums, new_cap * sizeof(seq_t));
    row_sums = new_sums;
    sums_cap = new_cap;
}

static void build_row(idx_t r)
{
    /* Build all rows up to and including row r */
    while (rows_cnt <= r) {
        idx_t i = rows_cnt;
        size_t len = (size_t)(i + 1);
        seq_t *cur = (seq_t *)malloc(len * sizeof(seq_t));
        /* T(i,0) = (-1)^i */
        cur[0] = ((i & 1) == 0) ? (seq_t)1 : (seq_t)-1;
        seq_t sum = cur[0];
        if (i >= 1) {
            seq_t *prev = rows[i - 1];
            seq_t sum_prev = row_sums[i - 1];
            cur[1] = sum_prev;          /* T(i,1) = sum of previous row */
            sum += cur[1];
            for (idx_t k = 2; k <= i; ++k) {
                cur[k] = cur[k - 1] - prev[k - 2];  /* recurrence */
                sum += cur[k];
            }
        }
        ensure_rows_cap(rows_cnt + 1);
        rows[rows_cnt] = cur;
        ensure_sums_cap(rows_cnt + 1);
        row_sums[rows_cnt] = sum;
        ++rows_cnt;
    }
}

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    build_row(r);
    idx_t k = n - offset_row(r);
    return rows[r][k];
}
