/*
 * A054081  Array T by antidiagonals: for n >= 0 and k >= 1, let p(k)=least
 *          positive integer not already in a(i), q(k)=p(k)+k+n-1,
 *          a(p(k))=q(k), a(q(k))=p(k); then for h>=1, T(n,h)=a(h).
 * https://oeis.org/A054081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, eigen
 * Formula:   construct the array T by antidiagonals as defined in A054081:
 *            For n ≥ 0 set p(k) = least positive integer not yet used as
 *            a(i), q(k) = p(k) + k + n – 1, a(p(k)) = q(k), a(q(k)) = p(k).
 *            Then T(n,h) = a(h). The sequence a(n) is T(0,n) listed by
 *            antidiagonal traversal
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   alloc_seq_array, diag_of, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/diag_of.h"
#include "../utils/free_seq_array.h"

/* ---------- helpers for possible arbitrary‑precision seq_t in C++ ---------- */
#ifdef __cplusplus

#else

#endif

/* ---------- global memoisation for each row ---------- */
static seq_t **row_vals = NULL;   /* arrays of a values for each row (size = col+1) */
static size_t *row_comp = NULL;   /* maximum column computed for each row */
static size_t rows_cnt = 0;       /* number of rows currently stored */

/* ---------- build the permutation for a given row up to a column ---------- */
static void build_row(idx_t r, idx_t max_col)
{
    /* a large enough upper bound to contain all values we will use */
    size_t limit = (size_t)max_col * 4 + (size_t)r + 10;
    bool *used = (bool*)calloc(limit + 1, sizeof(bool));
    if (!used) exit(1);

    seq_t *vals = alloc_seq_array((size_t)max_col + 1);
    if (!vals) {
        free(used);
        exit(1);
    }

    size_t assigned = 0;
    idx_t k = 1;
    idx_t p_candidate = 1;

    while (assigned < (size_t)max_col) {
        /* find least positive integer not yet used */
        while (p_candidate <= (idx_t)limit && used[p_candidate])
            ++p_candidate;
        idx_t p = p_candidate;
        idx_t q = p + k + r - 1;          /* q(k) = p(k) + k + n – 1 */

        if (p <= max_col && vals[p] == (seq_t)0) {
            vals[p] = (seq_t)q;
            ++assigned;
        }
        if (q <= max_col && vals[q] == (seq_t)0) {
            vals[q] = (seq_t)p;
            ++assigned;
        }

        if (p <= (idx_t)limit) used[p] = true;
        if (q <= (idx_t)limit) used[q] = true;
        ++k;
    }

    /* store the computed array, freeing old one if present */
    if (row_vals[r] != NULL)
        free_seq_array(row_vals[r]);

    row_vals[r] = vals;
    row_comp[r] = (size_t)max_col;

    free(used);
}

/* ---------- sequence a(n) ----------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) /* sequence is undefined before offset 1 */
        return (seq_t)0;

    /* find row and column in the antidiagonal order */
    idx_t s = diag_of(n);                            /* 0‑based diagonal */
    unsigned __int128 prev = (unsigned __int128)s * (s + 1) / 2;
    idx_t pos = (idx_t)(n - prev - 1);               /* 0‑based position in diagonal */
    idx_t row = pos;                                 /* 0‑based row index */
    idx_t col = s - pos + 1;                         /* 1‑based column index */

    /* ensure space for this row */
    if (row >= rows_cnt) {
        size_t new_sz = (size_t)row + 1;
        row_vals = (seq_t**)realloc(row_vals, new_sz * sizeof(seq_t*));
        row_comp = (size_t*)realloc(row_comp, new_sz * sizeof(size_t));
        for (size_t i = rows_cnt; i < new_sz; ++i) {
            row_vals[i] = NULL;
            row_comp[i] = 0;
        }
        rows_cnt = new_sz;
    }

    /* compute the row if necessary */
    if (row_comp[row] < (size_t)col)
        build_row(row, col);

    return row_vals[row][col];
}
