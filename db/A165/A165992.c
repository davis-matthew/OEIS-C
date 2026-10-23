/*
 * A165992  Triangle read by rows: T(n,0) = 3^n, T(n,k) = T(n,k-1) + T(n-1,k)
 *          for 0 < k < n, and T(n,n) = T(n,n-1).
 * https://oeis.org/A165992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle T with * T(r,0) = 3^r *
 *            T(r,k) = T(r,k-1) + T(r-1,k) (0 < k < r) * T(r,r) = T(r,r-1) *
 *            read row‑by‑row, left‑to‑right, top‑to‑bottom. T(0,0)=1. * The
 *            implementation builds rows on demand and memoises them
 * Range:     u8 n <= 15, u16 n <= 41, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq.h"

static seq_t **rows = NULL;      /* array of row pointers (each row is seq_t *) */
static size_t   rows_capacity = 0;
static idx_t    max_row = 0;      /* highest row built so far */
static seq_t    power3  = (seq_t)1;/* current 3^r */

/* Ensure rows[0..r] are available.  Builds missing rows incrementally. */
static void ensure_rows(idx_t r)
{
    /* Lazy initialisation on first use. */
    if (rows == NULL) {
        rows_capacity = 4;
        rows = (seq_t **)malloc(rows_capacity * sizeof(seq_t *));
        if (!rows) return;               /* allocation failure – subsequent use will crash */
        rows[0] = alloc_seq(1);          /* row 0 has 1 element */
        rows[0][0] = (seq_t)1;           /* T(0,0) = 1 */
        max_row  = 0;
        power3   = (seq_t)1;             /* 3^0 = 1 */
    }

    if (r <= max_row) return;           /* already present */

    /* Grow the array of row pointers if needed. */
    if (rows_capacity <= (size_t)r) {
        size_t new_cap = rows_capacity ? rows_capacity : 4;
        while (new_cap <= (size_t)r) new_cap <<= 1;
        seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        if (!tmp) return;               /* allocation failure – subsequent use will crash */
        rows = tmp;
        rows_capacity = new_cap;
    }

    /* Build rows up to r. */
    for (idx_t i = max_row + 1; i <= r; ++i) {
        seq_t *cur = alloc_seq(i + 1);   /* row i has i+1 elements */
        power3 = power3 * (seq_t)3;      /* 3^i */
        cur[0] = power3;                /* T(i,0) */

        /* Compute interior elements using the recurrence. */
        for (idx_t k = 1; k < i; ++k) {
            cur[k] = cur[k-1] + rows[i-1][k];
        }
        /* Last element of the row equals the previous one. */
        cur[i] = cur[i-1];

        rows[i] = cur;
    }
    max_row = r;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Determine the row r and column k containing index n. */
    idx_t r = 0;
    idx_t t = 0;                 /* starting index of row r */
    while (t + r + 1 <= n) {     /* while the next row would start ≤ n */
        t += r + 1;
        ++r;
    }
    idx_t k = n - t;             /* column within row r */

    ensure_rows(r);
    return rows[r][k];
}
