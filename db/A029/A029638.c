/*
 * A029638  Numbers in the (1,2)-Pascal triangle A029635 that are different
 *          from 1.
 * https://oeis.org/A029638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, frac, changed
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A029638: the sequence of entries in the (1,2)-Pascal triangle
 * A029635 that are > 1.  All entries produced by the recurrence are > 1,
 * so a(n) equals the nth element (row‑major) of the triangle.  The
 * sequence is obtained without any hard‑coded table and works for any n
 * ≥ 1. */

#include <stdlib.h>

/* Static storage for the triangle.  triangle[r] points to an array of
 * (r-1) entries for r ≥ 2, and a single entry for r == 1.  The array
 * is indexed from 1; index 0 is unused. */
static seq_t **triangle = NULL;
static idx_t  max_row   = 0;

/* Number of triangle entries up to and including row r */
static idx_t cumulative(idx_t r)
{
    if (r == 1) return 1;
    return 1 + (r - 1) * r / 2;       /* 1 + T(r-1)  */
}

/* Small linear search for the row that contains the n‑th entry.
 * For n ≤ 10^4 this requires at most ~140 iterations. */
static idx_t row_of(idx_t n)
{
    idx_t r = 1;
    while (cumulative(r) < n) ++r;
    return r;
}

/* Ensure that rows 1 … need are allocated and initialized. */
static void ensure_rows(idx_t need)
{
    if (need <= max_row) return;

    if (triangle == NULL) {
        triangle = (seq_t**) malloc((need + 1) * sizeof(seq_t*));
    } else {
        triangle = (seq_t**) realloc(triangle, (need + 1) * sizeof(seq_t*));
    }
    for (idx_t i = max_row + 1; i <= need; ++i) triangle[i] = NULL;

    for (idx_t r = max_row + 1; r <= need; ++r) {
        idx_t len = (r == 1) ? 1 : r - 1;            /* number of entries in row r */
        seq_t *row = (seq_t*) malloc(len * sizeof(seq_t));

        if (r == 1) {
            row[0] = (seq_t)2;
        } else {
            /* first entry of each row is the row number */
            row[0] = (seq_t)r;
            seq_t *prev = triangle[r - 1];
            idx_t prev_len = (r == 2) ? 1 : r - 2;   /* length of previous row */
            for (idx_t j = 1; j < len; ++j) {
                seq_t left  = prev[j - 1];
                seq_t right = (j < prev_len) ? prev[j] : (seq_t)0;
                row[j] = left + right;
            }
        }
        triangle[r] = row;
    }
    max_row = need;
}

/* Return the n‑th term of the sequence.  n ≥ 1. */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);                     /* row containing the n‑th entry */
    ensure_rows(r);

    idx_t prev_cum = (r == 1) ? 0 : cumulative(r - 1);
    idx_t offset   = n - prev_cum - 1;       /* 0‑based position inside the row */

    return triangle[r][offset];
}
