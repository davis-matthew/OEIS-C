/*
 * A136202  Triangle read by rows: expansion of Q(y, n), where Q(y,0)=1;
 *          Q(y,1)=y; Q(y, n) = -(-2 + 2*(1 - y) - 2*(1 - y)*Q(y, n - 1) +
 *          Q(y, n - 2)).
 * https://oeis.org/A136202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 18, i16 n <= 59, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A136202 - Triangle read by rows:
    Q(y,0)=1; Q(y,1)=y; Q(y,n) = -(-2 + 2*(1-y) - 2*(1-y)*Q(y,n-1) + Q(y,n-2))
    a(n) is the n-th element of this triangle (row-major order, offset 1). */

#include <stdlib.h>

/* Static cache for polynomial rows */
static seq_t **rows = NULL;          /* array of pointers to each row */
static size_t rows_cap = 0;          /* capacity of the rows array */
static size_t computed_rows = 0;     /* number of rows that have been computed */

/* Compute rows up to the requested index (inclusive). */
static void ensure_rows(idx_t up_to)
{
    if ((size_t)up_to < computed_rows)
        return;                          /* already available */

    /* Ensure the pointer array has enough capacity. */
    if (rows_cap <= (size_t)up_to + 1) {
        size_t new_cap = rows_cap ? rows_cap : 1;
        while (new_cap <= (size_t)up_to + 1)
            new_cap <<= 1;               /* double until enough */
        rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        rows_cap = new_cap;
    }

    /* Generate new rows one by one. */
    for (idx_t r = computed_rows; r <= up_to; ++r) {
        seq_t *arr = (seq_t*)malloc(sizeof(seq_t) * (r + 1));
        seq_t two = (seq_t)2;
        /* initialise to zero */
        for (idx_t i = 0; i <= r; ++i)
            arr[i] = 0;

        if (r == 0) {
            arr[0] = 1;
        } else if (r == 1) {
            arr[0] = 0;
            arr[1] = 1;
        } else {
            seq_t *prev     = rows[r - 1];   /* Q(y,r-1) */
            seq_t *prevprev = rows[r - 2];   /* Q(y,r-2) */

            /* (2 - 2y) * prev(y,r-1) */
            for (idx_t k = 0; k < r; ++k) {
                arr[k]     += two * prev[k];
                arr[k + 1] -= two * prev[k];
            }

            /* + 2y */
            arr[1] += two;

            /* - prevprev(y,r-2) */
            for (idx_t j = 0; j < r - 1; ++j)
                arr[j] -= prevprev[j];
        }
        rows[r] = arr;
    }

    computed_rows = (size_t)up_to + 1;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)          /* safety: for out-of-bounds requests */
        return 0;

    /* Find the row r containing the n-th element.  */
    idx_t r = 0;
    while (((r + 1) * (r + 2) / 2) < n)
        ++r;

    /* Index within that row (0‑based). */
    idx_t pos = n - r * (r + 1) / 2 - 1;

    ensure_rows(r);
    return rows[r][pos];
}
