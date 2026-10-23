/*
 * A028263  Elements in 3-Pascal triangle A028262 (by row) that are not 1.
 * https://oeis.org/A028263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth entry (row-major order) of the 3-Pascal
 *            triangle. The triangle T(n,k) (0 ≤ k ≤ n) is defined by T(n,0) =
 *            T(n,n) = n + 3 (edges) T(n,k) = T(n‑1,k‑1) + T(n‑1,k), 0 < k <
 *            n. (Pascal rule)
 * Range:     u8 n <= 38, u16 n <= 159, u32 n <= 575, u64 n <= 2239,
 *            u128 n <= 8575, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **rows = NULL;          /* pointers to each row */
static idx_t rows_alloc = 0;         /* allocated number of rows */
static idx_t rows_used = 0;          /* number of rows computed */

/* Compute cumulative term count up to (but excluding) row r:
       S(r) = r(r+1)/2   (sum of 1..r) */
static inline idx_t cumulative(idx_t r) {
    return (idx_t)((unsigned __int128)r * (r + 1) / 2);
}

/* Find the row index r such that cumulative(r) ≤ n < cumulative(r+1) */
static idx_t find_row(idx_t n) {
    idx_t low = 0, high = 1;
    while (cumulative(high) <= n)            /* exponential search */
        high <<= 1;
    while (low < high) {                     /* binary search */
        idx_t mid = low + (high - low) / 2;
        if (cumulative(mid) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;                           /* r */
}

/* Ensure that rows[0..r] are computed */
static void ensure_rows(idx_t r) {
    while (rows_used <= r) {
        idx_t i = rows_used;          /* next row to compute */
        /* allocate storage for the new row */
        seq_t *row = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        if (!row) abort();            /* allocation failure */

        if (i == 0) {
            row[0] = (seq_t)3;        /* first row */
        } else {
            seq_t edge = (seq_t)i + 3;
            row[0] = row[i] = edge;  /* edges */
            for (idx_t k = 1; k < i; ++k)
                row[k] = rows[i - 1][k - 1] + rows[i - 1][k];
        }
        rows[rows_used++] = row;
    }
}

/* Public interface: compute a(n) */
seq_t a(idx_t n) {
    /* determine row and column of term n */
    idx_t r = find_row(n);
    idx_t k = n - cumulative(r);

    /* compute missing rows up to r */
    if (rows_used <= r) {
        if (rows_alloc == 0) rows_alloc = 16;
        while (rows_alloc <= r) rows_alloc <<= 1;
        rows = (seq_t **)realloc(rows, rows_alloc * sizeof(seq_t *));
        if (!rows) abort();
    }
    ensure_rows(r);

    return rows[r][k];
}
