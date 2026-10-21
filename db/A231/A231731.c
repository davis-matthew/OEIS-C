/*
 * A231731  Triangular array read by rows: row n shows the coefficients of the
 *          polynomial u(n) = c(0) + c(1)*x + ... + c(n)*x^(n) which is the
 *          numerator of the n-th convergent of the continued fraction [k, k,
 *          k, ... ], where k = 2*x + 1.
 * https://oeis.org/A231731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   cumulative_before
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A231731: coefficients of the numerators of the convergents of the
   continued fraction [k, k, k, ...] with k = 2*x + 1.  The convergents p_n
   satisfy:
       p_{-1} = 1
       p_0   = 2*x + 1
       p_n   = (2*x + 1)*p_{n-1} + p_{n-2}      for n ≥ 1
   The sequence is read rowwise from the coefficient lists of p_{n}:
   row n (starting at 0) has n+2 coefficients (from degree 0 to n+1).
   a(1) = coefficient 0 of p_0, a(2) = coefficient 1 of p_0, etc.
*/

#include <stdint.h>
#include <stdlib.h>
#include "../utils/cumulative_before.h"

/* Binary search for the row containing the global index n (1‑based). */
static idx_t find_row(idx_t n) {
    idx_t lo = 0, hi = 1;
    while (cumulative_before(hi + 1) < n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (cumulative_before(mid + 1) >= n) hi = mid;
        else lo = mid + 1;
    }
    return lo;   /* 0‑based row index */
}

/* Dynamic storage for coefficients of rows p_n. */
static seq_t **rows = NULL;          /* rows[0] = p_0, rows[1] = p_1, ...   */
static idx_t rows_allocated = 0;     /* allocated capacity in rows */
static idx_t rows_computed  = 0;     /* how many rows have been computed */

/* Ensure space for at least 'need' pointers in 'rows'. */
static void ensure_rows_capacity(idx_t need) {
    if (rows_allocated >= need) return;
    idx_t new_cap = rows_allocated ? rows_allocated << 1 : 8;
    if (new_cap < need) new_cap = need;
    seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
    rows = tmp;
    rows_allocated = new_cap;
}

/* Compute row 'r' (0‑based) and store it in rows[r]. */
static void compute_row(idx_t r) {
    /* Allocate capacity first. */
    rows_computed = r + 1;
    rows[rows_computed - 1] = NULL;
    rows_computed = r + 1;  /* just to keep compiler happy */

    /* General lengths */
    idx_t len_cur = r + 2;                      /* number of coefficients in row r */
    seq_t *cur = (seq_t *)malloc(len_cur * sizeof(seq_t));
    rows[r] = cur;

    if (r == 0) {
        /* p_0 = 2*x + 1  => coefficients [1, 2] */
        cur[0] = 1;
        cur[1] = 2;
        return;
    }

    /* Retrieve previous rows */
    idx_t len_prev1 = (r - 1) + 2;              /* length of row r-1 */
    seq_t *prev1 = rows[r - 1];
    idx_t len_prev2 = (r - 2) + 2;              /* length of row r-2, or 1 if r==1 */
    seq_t *prev2 = (r == 1) ? NULL : rows[r - 2];

    /* Multiply prev1 by k = 1 + 2*x */
    /* The convolution: (1, 2) * prev1 */
    /* Result length = len_prev1 + 1 = len_cur */
    cur[0] = prev1[0];                 /* prev1[0] * 1 */

    for (idx_t i = 1; i < len_prev1; ++i) {
        cur[i] = prev1[i] + prev1[i - 1] * 2;
    }
    cur[len_prev1] = prev1[len_prev1 - 1] * 2;   /* last term */

    /* Add prev2 if it exists */
    if (prev2 != NULL) {
        /* prev2 has length len_prev2 = r */
        for (idx_t i = 0; i < len_prev2; ++i) {
            cur[i] += prev2[i];
        }
    } else {
        /* r == 1: add constant 1 to cur[0] (u(-1) = 1) */
        cur[0] += 1;
    }
}

/* Ensure that rows up to 'r' (inclusive) are computed. */
static void ensure_rows_upto(idx_t r) {
    if (rows == NULL) {
        ensure_rows_capacity(r + 1);
        rows_allocated = r + 1;
    }
    while (rows_computed <= r) {
        compute_row(rows_computed);
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset is 1, but safety */
    /* Find row and offset within row. */
    idx_t r = find_row(n);          /* 0‑based row index */
    idx_t start = cumulative_before(r);   /* number of terms before this row */
    idx_t offset = n - start - 1;        /* 0‑based within row r */

    /* Ensure rows up to r are ready */
    ensure_rows_upto(r);

    return rows[r][offset];
}
