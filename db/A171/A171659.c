/*
 * A171659  Lucas-version of A165293.
 * https://oeis.org/A171659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, more
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 21, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A171659 – Lucas‑version of A165293
 *
 * The triangle T(i,j) (i ≥ 1, 1 ≤ j ≤ i) is defined by
 *
 *   T(1,1)           = 2
 *   T(2,1)           = 10
 *   T(i,1)           = 10 × T(i‑1,1)        for i ≥ 3
 *   T(i,i)           = (‑1)^(i‑1) × 2
 *   T(i,j)           = 10 × T(i‑1,j) – T(i‑1,j‑1)   for 1 < j < i
 *
 * a(n) is the triangle read in row‑major order:
 *   a(1) = 2, a(2) = 10, a(3) = –2, a(4) = 100, …
 *
 * The implementation stores the triangle in a dynamic array.
 * Any a(n) is retrieved in O(r) time where r ≈ √(2 n).
 */

#include <stdlib.h>

/* Cached data for the triangle */
static seq_t *tri_data  = NULL;
static size_t tri_cap   = 0;   /* capacity of tri_data buffer   */
static size_t tri_len   = 0;   /* number of elements stored     */

static size_t *row_start = NULL;   /* row_start[1] = offset of row 1 in tri_data */
static size_t start_cap  = 0;      /* capacity of row_start array    */
static size_t rows       = 0;      /* number of rows already built   */

/* Ensure the triangle contains at least rows `r`. */
static void ensure_row(idx_t r)
{
    /* Grow row_start if necessary */
    if ((size_t)r >= start_cap) {
        size_t new_cap = (start_cap == 0) ? 16 : start_cap * 2;
        size_t *tmp = (size_t *)realloc(row_start, new_cap * sizeof(size_t));
        if (!tmp) abort();
        row_start = tmp;
        start_cap = new_cap;
    }

    /* Grow data buffer if necessary */
    size_t needed = tri_len + (size_t)r;
    if (needed > tri_cap) {
        size_t new_cap = (tri_cap == 0) ? 64 : tri_cap;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(tri_data, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        tri_data = tmp;
        tri_cap  = new_cap;
    }

    /* Mark start of new row */
    row_start[r] = tri_len;

    /* Generate the row */
    idx_t i = r;                 /* row number (1‑based) */
    if (i == 1) {
        tri_data[tri_len++] = (seq_t)2;
        return;
    }

    idx_t prev_row = i - 1;
    size_t prev_start = row_start[prev_row];

    for (size_t j = 0; j < (size_t)i; ++j) {
        seq_t val;
        if (j == 0) { /* first column */
            if (i == 2)
                val = (seq_t)10;                /* special case for row 2 */
            else
                val = tri_data[prev_start] * (seq_t)10;
        } else if (j == (size_t)i - 1) { /* last column */
            val = ((i - 1) & 1) ? (seq_t)-2 : (seq_t)2;
        } else { /* interior element */
            val = tri_data[prev_start + j] * (seq_t)10
                - tri_data[prev_start + j - 1];
        }
        tri_data[tri_len++] = val;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* safety for off‑by‑one calls */

    /* Find row r that contains position n (1‑based). */
    size_t cumulative = 0;
    size_t r = 0;
    while (cumulative < (size_t)n) {
        ++r;
        cumulative += r;
    }

    /* Grow the triangle as needed. */
    while (rows < r) {
        ++rows;
        ensure_row(rows);
    }

    /* Compute the offset within the row. */
    size_t prev_cumulative = cumulative - r;   /* sum of rows 1 .. r‑1 */
    size_t offset = (size_t)(n - prev_cumulative - 1);  /* 0‑based */

    return tri_data[row_start[r] + offset];
}
