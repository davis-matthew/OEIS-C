/*
 * A064642  Triangle defined in A064641 read by rows.
 * https://oeis.org/A064642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 42, u32 n <= 131, u64 n <= 402,
 *            u128 n <= 1375, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle defined by A064641, read by rows (row‑major).
   Recurrence:
      P(0,0)     = 1
      P(i,0)     = 1          for i ≥ 1
      P(i,j) = P(i‑1,j) + P(i‑1,j‑1) + P(i,j‑1) + P(i‑2,j‑1)
      with missing terms treated as 0.  The sequence a(n)
   is the nth element when the triangle is read row by row. */
#include <stdlib.h>
#include <stdint.h>

static seq_t **triangle = NULL;   /* array of pointers to rows */
static size_t rows = 0;           /* number of rows already built */

/* Allocate and fill rows [rows..i] of the triangle. */
static void ensure_rows_up_to(size_t i)
{
    if (i < rows) return;               /* already built */

    /* Grow the outer array of row pointers. */
    triangle = (seq_t **)realloc(triangle, (i + 1) * sizeof(seq_t *));
    if (!triangle) abort();             /* out of memory */

    for (size_t r = rows; r <= i; ++r) {
        triangle[r] = (seq_t *)malloc((r + 1) * sizeof(seq_t));
        if (!triangle[r]) abort();     /* out of memory */

        triangle[r][0] = 1;                 /* first element of each row */

        for (size_t c = 1; c <= r; ++c) {
            seq_t a1 = 0;           /* P(r-1,c) */
            if (r >= 1 && c <= r - 1) a1 = triangle[r - 1][c];

            seq_t a2 = triangle[r - 1][c - 1];   /* P(r-1,c-1) exists for c≥1 */

            seq_t a3 = triangle[r][c - 1];       /* P(r,c-1) exists */

            seq_t a4 = 0;           /* P(r-2,c-1) */
            if (r >= 2 && c <= r - 1) a4 = triangle[r - 2][c - 1];

            triangle[r][c] = a1 + a2 + a3 + a4;
        }
    }
    rows = i + 1;                        /* rows 0 … i are now available */
}

/* Return the nth term of sequence A064642. */
seq_t a(idx_t n)
{
    /* Find row index i such that T(i) ≤ n < T(i+1),  T(i)=i*(i+1)/2. */
    size_t i = 0;
    while (((idx_t)(i + 1) * (i + 2) / 2) <= n)
        ++i;

    idx_t row = i;
    idx_t t   = ((idx_t)i * ((idx_t)i + 1)) / 2;   /* triangular number T(i) */
    idx_t col = n - t;                            /* column within the row */

    ensure_rows_up_to(row);
    return triangle[row][(size_t)col];
}
