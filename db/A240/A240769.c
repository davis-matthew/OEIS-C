/*
 * A240769  Triangle read by rows: T(1,1) = 1; T(n+1,k) = T(n,k+1), 1 <= k <
 *          n; T(n+1,n) = 2*T(n,1); T(n+1,n+1) = 2*T(n,1) - 1.
 * https://oeis.org/A240769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   OEIS A240769 * Triangle read by rows: * T(1,1) = 1; * T(n+1,k) =
 *            T(n,k+1), 1 ≤ k < n; * T(n+1,n) = 2*T(n,1); * T(n+1,n+1) =
 *            2*T(n,1) – 1
 * Range:     u8 n <= 8256, u16 n <= 8256, u32 n <= 8256, u64 n <= 8256,
 *            u128 n <= 8256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **g_rows = NULL;        /* g_rows[i] holds row i+1 */
static size_t g_rows_cnt = 0;        /* number of rows currently stored */
static size_t g_rows_cap = 0;        /* capacity of rows array */

/* Ensure rows up to 'row' (1‑based) are computed. */
static void ensure_rows(idx_t row)
{
    /* compute the first row if necessary */
    if (g_rows_cnt == 0) {
        seq_t *row1 = (seq_t*)malloc(sizeof(seq_t));
        if (!row1) abort();
        row1[0] = (seq_t)1;
        g_rows = (seq_t**)malloc(sizeof(seq_t*));
        if (!g_rows) abort();
        g_rows[0] = row1;
        g_rows_cnt = 1;
        g_rows_cap = 1;
    }

    /* iteratively generate further rows */
    while (g_rows_cnt < row) {
        idx_t old_len = g_rows_cnt;          /* length of the previous row */
        seq_t *prev = g_rows[old_len-1];
        idx_t new_len = old_len + 1;         /* length of the new row */

        seq_t *new_row = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!new_row) abort();

        /* Shift the inner part of the row */
        if (new_len >= 3) {
            for (idx_t i = 0; i + 2 < new_len; ++i)
                new_row[i] = prev[i+1];
        }

        seq_t first = prev[0];
        new_row[new_len-2] = (seq_t)2 * first;
        new_row[new_len-1] = (seq_t)2 * first - (seq_t)1;

        /* Grow the rows array if necessary */
        if (g_rows_cnt >= g_rows_cap) {
            size_t new_cap = g_rows_cap ? g_rows_cap * 2 : 8;
            g_rows = (seq_t**)realloc(g_rows, new_cap * sizeof(seq_t*));
            if (!g_rows) abort();
            g_rows_cap = new_cap;
        }

        g_rows[g_rows_cnt] = new_row;
        ++g_rows_cnt;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined, but harmless */

    /* find the row r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    /* compute required rows */
    ensure_rows(r);

    /* position within the row (1‑based) */
    idx_t pos = n - (r - 1) * r / 2;   /* 1 ≤ pos ≤ r */

    return g_rows[r-1][pos-1];
}
