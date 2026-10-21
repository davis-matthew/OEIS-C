/*
 * A217770  Square array T, read by antidiagonals: T(n,k) = 0 if n-k >=4 or if
 *          k-n >= 6, T(3,0) = T(2,0) = T(1,0) = T(0,0) = T(0,1) = T(0,2) =
 *          T(0,3) = T(0,4) = T(0,5) = 1, T(n,k) = T(n-1,k) + T(n,k-1).
 * https://oeis.org/A217770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   the sequence a(n) is read from the two‑dimensional array T *
 *            defined by * * T(n,k) = 0 if n-k >= 4 or k-n >= 6 * T(0,0) =
 *            T(1,0) = T(2,0) = T(3,0) = 1 * T(0,1) = T(0,2) = T(0,3) = T(0,4)
 *            = T(0,5) = 1 * else T(n,k) = T(n-1,k) + T(n,k-1) * * The
 *            sequence a is obtained by traversing T along its anti...
 * Range:     u8 n <= 69, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* internal storage -------------------------------------------------- */
/* flattened sequence values */
static seq_t *a_buf   = NULL;
static size_t a_size  = 0;  /* number of values stored so far     */
static size_t a_cap   = 0;  /* allocated capacity of a_buf         */

/* 2‑D table: rows of pointers to columns */
static seq_t **T            = NULL;   /* rows of the table                 */
static size_t *row_cap      = NULL;   /* allocated column length per row    */
static size_t tbl_rows_cap  = 0;      /* how many rows are currently allocated */

/* last processed antidiagonal index (sum of coordinates) */
static int64_t cur_sum = -1;

/* ------------------------------------------------------------------ */
/* helper functions ------------------------------------------------- */
/* ensure a row can hold at least len columns */
static void ensure_row_length(size_t row, size_t len)
{
    if (row >= tbl_rows_cap)
        abort();  /* allocated rows must be >= requested row */

    if (row_cap[row] >= len)
        return;

    seq_t *tmp = (seq_t *)realloc(T[row], len * sizeof(seq_t));
    if (!tmp)
        abort();
    T[row] = tmp;
    row_cap[row] = len;
}

/* ensure enough row pointers are allocated */
static void ensure_table_rows(size_t needed_rows)
{
    if (needed_rows <= tbl_rows_cap)
        return;

    seq_t **tmp_rows = (seq_t **)realloc(T, needed_rows * sizeof(seq_t *));
    if (!tmp_rows)
        abort();
    T = tmp_rows;

    size_t *tmp_cap = (size_t *)realloc(row_cap, needed_rows * sizeof(size_t));
    if (!tmp_cap)
        abort();
    row_cap = tmp_cap;

    for (size_t i = tbl_rows_cap; i < needed_rows; ++i) {
        T[i]       = NULL;
        row_cap[i] = 0;
    }
    tbl_rows_cap = needed_rows;
}

/* extend the pre‑computation to include a(n) */
static void extend_to_n(idx_t n)
{
    size_t target = (size_t)n;
    while (a_size <= target) {
        size_t s = (size_t)(cur_sum + 1);          /* next antidiagonal sum */

        /* rows 0 .. s inclusive are needed */
        ensure_table_rows(s + 1);

        /* each row must hold columns 0 .. s */
        for (size_t i = 0; i <= s; ++i)
            ensure_row_length(i, s + 1);

        /* compute all entries on this diagonal */
        for (size_t i = 0; i <= s; ++i) {
            size_t j   = s - i;
            seq_t val;

            int diff = (int)((intptr_t)i - (intptr_t)j);  /* i-j may fit in int */

            if (diff >= 4 || diff <= -6) {
                val = 0;
            } else if (i == 0 && j == 0) {
                val = 1;
            } else if (i == 0) {          /* k == 0 row */
                val = (j <= 5) ? 1 : 0;
            } else if (j == 0) {          /* i > 0 column 0 */
                val = (i <= 3) ? 1 : 0;
            } else {
                val = T[i - 1][j] + T[i][j - 1];
            }

            T[i][j] = val;

            /* append to flattened array */
            if (a_size >= a_cap) {
                size_t new_cap = (a_cap == 0) ? 8 : a_cap * 2;
                seq_t *tmp = (seq_t *)realloc(a_buf, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                a_buf = tmp;
                a_cap = new_cap;
            }
            a_buf[a_size++] = val;
        }
        cur_sum = (int64_t)s;
    }
}

/* ------------------------------------------------------------------ */
/* public routine ---------------------------------------------------- */
seq_t a(idx_t n)
{
    extend_to_n(n);
    return a_buf[(size_t)n];
}
