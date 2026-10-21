/*
 * A238763  A Motzkin triangle read by rows, 0<=k<=n.
 * https://oeis.org/A238763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the Motzkin triangle * read by rows,
 *            0 <= k <= n. * Let r be the row index such that r(r+1)/2 <= n <
 *            (r+1)(r+2)/2, * and let k = n - r(r+1)/2. * Then a(n) = T(k, r),
 *            where T(p,q) is defined by * * T(0,0) = 1 * T(p,q) = 0 if p < 0
 *            or p > q * T(p,q) = T(p-2,q) + T(p-1,q-1...
 * Range:     u8 n <= 43, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **tri_rows = NULL;      /* array of pointers to rows 0..R */
static size_t   tri_cap  = 0;        /* number of allocated rows */

static void compute_row(idx_t q)
{
    size_t i = (size_t)q;
    seq_t *row = (seq_t*)calloc(i + 1, sizeof(seq_t));
    /* Row 0 special case: T(0,0)=1 */
    if (i == 0) {
        row[0] = (seq_t)1;
        tri_rows[i] = row;
        return;
    }

    /* For q >= 1, compute elements 0..q using recurrence. */
    for (size_t p = 0; p <= i; ++p) {
        seq_t val = 0;

        /* T(p-2,q)  -> same row, earlier element */
        if (p >= 2)
            val += row[p - 2];

        /* T(p-1,q-1) -> previous row */
        if (p >= 1)
            val += tri_rows[i - 1][p - 1];

        /* T(p,q-2)  -> row q-2 */
        if (i >= 2 && p <= i - 2)
            val += tri_rows[i - 2][p];

        row[p] = val;
    }

    tri_rows[i] = row;
}

/* Ensure rows 0..q are computed */
static void ensure_rows(idx_t q)
{
    size_t need = (size_t)q + 1;
    if (tri_cap < need) {
        seq_t **new_rows = (seq_t**)realloc(tri_rows, need * sizeof(seq_t*));
        if (!new_rows) return; /* allocation failed: leave it */
        tri_rows = new_rows;
        for (size_t i = tri_cap; i < need; ++i)
            compute_row((idx_t)i);
        tri_cap = need;
    }
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Ensure needed rows are available */
    ensure_rows(r);

    /* Index within the row */
    idx_t prev_cnt = r * (r + 1) / 2;
    idx_t k = n - prev_cnt;   /* 0 <= k <= r */

    return tri_rows[(size_t)r][(size_t)k];
}
