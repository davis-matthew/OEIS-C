/*
 * A135835  Lower triangular matrix T with first column and diagonal
 *          (1,2,3,4,...,n,...) and otherwise satisfying T(i,j) = Sum_{k=1..j}
 *          T(i-j+1,k)*T(j,k), read by rows.
 * https://oeis.org/A135835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th entry of the lower‑triangular matrix T * read
 *            by rows. T satisfies: * T(i,1) = T(i,i) = i * For 1<j<i: *
 *            T(i,j) = sum_{k=1}^{min(j,i-j+1)} T(i-j+1,k) * T(j,k) * * The
 *            sequence a is: 1; 2,2; 3,8,3; 4,22,22,4; …
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 180, u64 n <= 647,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for the computed rows of T */
static idx_t     max_row = 0;      /* highest row computed so far   */
static size_t    tri_cap = 0;      /* capacity of the pointer array */
static seq_t **tri   = NULL;       /* tri[i] points to row i (1‑based) */

/* ------------------------------------------------------------------ */
/* Find the row index r such that r*(r+1)/2 >= n.  Binary search.      */
static idx_t
find_row(idx_t n)
{
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t val = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (val < (seq_t)n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/* Ensure that rows up to r are computed.  This function allocates     */
/* new rows and stores them in the static array tri[].                 */
static void
ensure_rows(idx_t r)
{
    if (r <= max_row)
        return;

    /* Grow the pointer array if necessary */
    if (r + 1 > tri_cap) {
        size_t new_cap = r + 1;
        sizeof(char); /* dummy to avoid unused warning if compile with -Werror=unused-variable */
        tri = (seq_t**)realloc(tri, new_cap * sizeof(seq_t*));
        tri_cap = new_cap;
    }

    for (idx_t i = max_row + 1; i <= r; ++i) {
        /* allocate row i (1‑based indexing; ignore 0) */
        tri[i] = (seq_t*)malloc((i + 1) * sizeof(seq_t));
        /* compute entries of row i */
        for (idx_t k = 1; k <= i; ++k) {
            if (k == 1 || k == i) {
                tri[i][k] = (seq_t)i;
            } else {
                seq_t sum = 0;
                idx_t limit = i - k + 1;
                if (limit > k)
                    limit = k;
                for (idx_t j = 1; j <= limit; ++j) {
                    sum += tri[i - k + 1][j] * tri[k][j];
                }
                tri[i][k] = sum;
            }
        }
    }
    max_row = r;
}

/* ------------------------------------------------------------------ */
/* Public function required by the problem.                            */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset is 1; below it is undefined */

    /* find row and column of the n-th term */
    idx_t row = find_row(n);
    seq_t offset = (seq_t)(row - 1) * (seq_t)row / 2;
    idx_t col = (idx_t)(n - offset);    /* 1‑based column */

    ensure_rows(row);
    return tri[row][col];
}
