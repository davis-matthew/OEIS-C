/*
 * A121531  Triangle read by rows: T(n,k) is the number of nondecreasing Dyck
 *          paths of semilength n and having k double rises at an even level
 *          (n >= 1, k >= 0). A nondecreasing Dyck path is a Dyck path for
 *          which the sequence of the altitudes of the valleys is
 *          nondecreasing.
 * https://oeis.org/A121531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   add_to_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: for n ≥ 1 and k ≥ 0

   T(n,k) = 2·T(n‑1,k)           + 4·T(n‑2,k‑1)
            –  T(n‑3,k)          – 3·T(n‑3,k‑1)
            – 2·T(n‑4,k‑1)       – 4·T(n‑4,k‑2)
            +   T(n‑6,k‑3)       + N(n,k),

   where N(n,k) is non‑zero only for (n,k) = (1,0),(3,1),(4,1),(5,2),(6,2);
   N(1,0)=1, N(3,1)=–3, N(4,1)=–1, N(5,2)=2, N(6,2)=1.

   Row n contains ceil(n/2) terms T(n,0)…T(n,⌈n/2⌉–1).  The function
   a(idx_t n) returns the n‑th element of the triangle read by rows
   (1‑based indexing).  Initial rows:

      T(1,0)=1
      T(2,0)=2
*/

#include <stdlib.h>
#include "../utils/add_to_array.h"

/* The types seq_t and idx_t are supplied by the build environment.  They
   represent arbitrary‑precision unsigned integers and 64‑bit indices,
   respectively.  No redeclaration of these types is performed here.  */

static idx_t      last_row      = 0;   /* highest computed row */
static size_t     rows_cap      = 0;   /* current size of the row arrays */
static seq_t    **rows          = NULL;/* rows[1..last_row] -> row arrays */
static idx_t     *row_len       = NULL;/* row_len[n] = length of row n (ceil(n/2)) */
static idx_t     *row_start     = NULL;/* starting index of each row in the flat array */
static idx_t      total_terms   = 0;   /* total number of terms up to last_row */

/* Ensure that the arrays can hold at least rows up to n (1‑based). */
static void ensure_rows_capacity(idx_t n)
{
    if (n > rows_cap) {
        size_t new_cap = (n + 11) * 2;               /* overshoot for growth */
        rows      = (seq_t **)realloc(rows,          (new_cap + 1) * sizeof(seq_t *));
        row_len   = (idx_t   *)realloc(row_len,      (new_cap + 1) * sizeof(idx_t));
        row_start = (idx_t   *)realloc(row_start,    (new_cap + 1) * sizeof(idx_t));
        rows_cap  = new_cap;
    }
}

/* Compute row n (1‑based) and store it in rows[n]. */
static void compute_next_row(idx_t n)
{
    ensure_rows_capacity(n);
    idx_t len = (n + 1) >> 1;                   /* ceil(n/2) */
    rows[n]   = (seq_t *)calloc(len, sizeof(seq_t));
    row_len[n] = len;

    /* Base rows */
    if (n == 1) { rows[1][0] = 1; return; }
    if (n == 2) { rows[2][0] = 2; return; }

    /* Pointers to required previous rows (may be NULL). */
    const seq_t *a1 = (n >= 1) ? rows[n-1] : NULL;
    const seq_t *a2 = (n >= 2) ? rows[n-2] : NULL;
    const seq_t *a3 = (n >= 3) ? rows[n-3] : NULL;
    const seq_t *a4 = (n >= 4) ? rows[n-4] : NULL;
    const seq_t *a6 = (n >= 6) ? rows[n-6] : NULL;

    /* Lengths of those rows. */
    const idx_t  len1 = (a1) ? row_len[n-1] : 0;
    const idx_t  len2 = (a2) ? row_len[n-2] : 0;
    const idx_t  len3 = (a3) ? row_len[n-3] : 0;
    const idx_t  len4 = (a4) ? row_len[n-4] : 0;
    const idx_t  len6 = (a6) ? row_len[n-6] : 0;

    seq_t *res   = rows[n];           /* positive contributions */
    seq_t *neg   = (seq_t *)calloc(len, sizeof(seq_t)); /* negative contributions */

    /* Positive terms: 2·T(n-1,k)  + 4·T(n-2,k-1)  + T(n-6,k-3) */
    add_to_array(res, len, a1, len1, 0, 2);
    add_to_array(res, len, a2, len2, 1, 4);
    add_to_array(res, len, a6, len6, 3, 1);

    /* Negative terms: -T(n-3,k) -3·T(n-3,k-1) -2·T(n-4,k-1) -4·T(n-4,k-2) */
    add_to_array(neg, len, a3, len3, 0, 1);
    add_to_array(neg, len, a3, len3, 1, 3);
    add_to_array(neg, len, a4, len4, 1, 2);
    add_to_array(neg, len, a4, len4, 2, 4);

    /* N(n,k) contributions */
    if (n == 1) { res[0] += 1; }
    else if (n == 3) { neg[1] += 3; }          /* -3 at k=1 */
    else if (n == 4) { neg[1] += 1; }          /* -1 at k=1 */
    else if (n == 5) { res[2] += 2; }          /* +2 at k=2 */
    else if (n == 6) { res[2] += 1; }          /* +1 at k=2 */

    /* Final subtraction of negative contributions.  All values are
       guaranteed non‑negative after this step. */
    for (idx_t j = 0; j < len; ++j) {
        res[j] -= neg[j];
    }
    free(neg);
}

/* Ensure that rows up to the required flattened index are computed. */
static void ensure_rows_for_idx(idx_t pos)
{
    while (total_terms < pos) {
        ++last_row;
        compute_next_row(last_row);
        row_start[last_row] = total_terms;  /* first index of this row */
        total_terms += row_len[last_row];
    }
}

/* Locate the row that contains the flattened index pos (1‑based). */
static idx_t locate_row(idx_t pos)
{
    idx_t lo = 1, hi = last_row;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (row_start[mid] + row_len[mid] >= pos)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Public entry point: compute the n-th term of the flattened triangle
   (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety: offset 1 */
    ensure_rows_for_idx(n);
    idx_t row = locate_row(n);
    idx_t index_in_row = n - row_start[row] - 1; /* 0‑based */
    return rows[row][index_in_row];
}
