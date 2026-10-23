/*
 * A117918  Difference row triangle of the Pell sequence.
 * https://oeis.org/A117918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 104, u32 n <= 376, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A117918 – Difference row triangle of the Pell sequence.
 *
 * The triangle T(n,k) is defined by
 *   T(1,1) = 1
 *   T(n,1) = 2^⌊(n-1)/2⌋
 *   T(n,k) = T(n,k-1) + T(n-1,k-1)   (2 ≤ k ≤ n)
 *   T(n,n) = Pell(n)                (A000129)
 *
 * The required sequence a(n) is the row‑by‑row reading of this triangle
 * (row major, starting at a(1)=T(1,1)).  Hence a(n)=T(r,c) where r is the
 * smallest integer with r(r+1)/2 ≥ n and c = n − (r−1)r/2.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cached rows of the triangle.  tri[i] points to an array of size i+1
 * (indices 0..i) where only elements 1..i are used.  tri[0] is NULL.
 */
static seq_t **tri = NULL;
static idx_t rows_alloc = 0;

/* Ensure rows 1..r are computed and stored. */
static void ensure_rows(idx_t r)
{
    if (r <= rows_alloc)
        return;

    /* Expand the array of row pointers. */
    seq_t **new_tri = (seq_t**)realloc(tri, (r + 1) * sizeof(seq_t*));
    if (new_tri == NULL)
        abort();
    tri = new_tri;

    for (idx_t i = rows_alloc + 1; i <= r; ++i) {
        /* Allocate storage for row i (indices 0..i). */
        seq_t *row = (seq_t*)malloc((i + 1) * sizeof(seq_t));
        if (row == NULL)
            abort();

        /* T(i,1) = 2^floor((i-1)/2).  Compute by repeated doubling to
         * avoid unsigned shift overflow on very small seq_t widths. */
        idx_t shift = (i - 1) / 2;
        seq_t val = 1;
        for (idx_t s = 0; s < shift; ++s)
            val *= 2;
        row[1] = val;

        /* Remaining entries via the recurrence. */
        for (idx_t k = 2; k <= i; ++k) {
            /* For k=2 we use tri[i-1][1] which is defined because i>1. */
            row[k] = row[k - 1] + tri[i - 1][k - 1];
        }

        tri[i] = row;
    }
    rows_alloc = r;
}

/* Convert linear index n (1‑based) into row r and column c of the triangle. */
static void index_to_rc(idx_t n, idx_t *r_out, idx_t *c_out)
{
    idx_t r = 0;
    idx_t acc = 0;
    while (acc < n) {
        ++r;
        acc += r;                /* acc = r(r+1)/2 after increment */
    }
    idx_t c = n - (acc - r);     /* acc - r = (r-1)r/2 */
    *r_out = r;
    *c_out = c;
}

/* Return the nth term of sequence A117918 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* Undefined in OEIS, but keeps the function total. */

    idx_t r, c;
    index_to_rc(n, &r, &c);
    ensure_rows(r);
    return tri[r][c];
}
