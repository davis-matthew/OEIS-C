/*
 * A132057  A convolution triangle of numbers obtained from A034904.
 * https://oeis.org/A132057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n,m) = 7*(7*(n-1)+m)*a(n-1,m)/n + m*a(n-1,m-1)/n, with
 *            a(n,0)=0, a(n,m)=0 for n<m, a(1,1)=1. Sequence a(n) enumerates
 *            the triangle row‑wise: a(1)=a(1,1), a(2)=a(2,1), a(3)=a(2,2),
 *            a(4)=a(3,1), …
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 21, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   linidx_to_rc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/linidx_to_rc.h"

/* static cache of the triangle, one row per pointer (1‑based indexing) */
static seq_t **tri = NULL;     /* tri[r] points to row r array */
static idx_t rows_alloced = 0; /* highest row currently allocated */

/* Allocate and compute rows up to 'rows_needed' */
static void ensure_rows(idx_t rows_needed)
{
    if (rows_needed <= rows_alloced)
        return;

    /* Grow the row pointer array */
    tri = (seq_t **)realloc(tri, (rows_needed + 1) * sizeof(*tri));

    for (idx_t r = rows_alloced + 1; r <= rows_needed; ++r) {
        /* allocate row r (ignore index 0) */
        tri[r] = (seq_t *)calloc(r + 1, sizeof(seq_t));

        if (r == 1) {             /* base case */
            tri[1][1] = 1;
            continue;
        }

        /* compute entries 1 .. r-1 using recurrence */
        for (idx_t m = 1; m < r; ++m) {
            seq_t prev_m     = tri[r - 1][m];
            seq_t prev_m_1   = tri[r - 1][m - 1];
            /* factor = 7*(7*(r-1)+m) */
            seq_t factor = (seq_t)7 * (((seq_t)7 * ((seq_t)(r - 1))) + m);
            seq_t numer  = factor * prev_m;          /* 7*(7*(r-1)+m)*a(r-1,m) */
            numer += (seq_t)m * prev_m_1;           /* + m*a(r-1,m-1) */
            tri[r][m] = numer / r;                 /* integer division */
        }
        tri[r][r] = 1; /* a(r,r)=1 by definition */
    }
    rows_alloced = rows_needed;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n=0 but safe */

    idx_t row, col;
    linidx_to_rc(n, &row, &col);

    ensure_rows(row);
    return tri[row][col];
}
