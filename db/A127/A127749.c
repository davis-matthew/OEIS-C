/*
 * A127749  Inverse of number triangle A(n,k) = 1/(2n+1) if k <= n <= 2k, 0
 *          otherwise.
 * https://oeis.org/A127749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 68, i16 n <= 68, i32 n <= 68, i64 n <= 68, i128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A127749 – inverse triangle of 1/(2n+1).
    The values of the triangle B(n,k) satisfy

        B(n,k) = (2n+1)      , if k = n
        B(n,k) = 0,           otherwise

      B(n,k) = - Σ_{l=⌈n/2⌉}^{n-1} B(l,k)      (k ≠ n)
      B(n,n) = (2n+1) - Σ_{l=⌈n/2⌉}^{n-1} B(l,n)

    The sequence a(n) is obtained by listing the rows of B in
    row‑major order: a(0) = B(0,0), a(1) = B(1,0), a(2) = B(1,1),
    a(3) = B(2,0), a(4) = B(2,1), a(5) = B(2,2), …          */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  The triangle is built incrementally and cached between calls   */
    static seq_t **tri = NULL;      /* pointer array of rows          */
    static idx_t  tri_cap = 0;      /* current capacity of pointer array */
    static idx_t  next_row = 0;     /* next row that has not yet been computed */

    /*  Determine the row r and column c that correspond to linear index n   */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;          /* find smallest r with (r+1)(r+2)/2 > n */
    idx_t entries_before = r * (r + 1) / 2;
    idx_t c = n - entries_before;                     /* 0 ≤ c ≤ r */

    /*  Ensure the pointer array is large enough   */
    if (r + 1 > tri_cap) {
        size_t new_cap = (size_t)(r + 1);
        tri = (seq_t **)realloc(tri, new_cap * sizeof(seq_t *));
        tri_cap = (idx_t)new_cap;
    }

    /*  Compute any missing rows up to r   */
    for (idx_t i = next_row; i <= r; ++i) {
        /*  Allocate the i‑th row (length i+1)   */
        tri[i] = (seq_t *)malloc((size_t)(i + 1) * sizeof(seq_t));

        /*  For each column j in this row, accumulate the sum of previous
            entries B(l,j) for l from ceil(i/2) to i-1, using the recurrence   */
        idx_t start_l = (i + 1) / 2;            /* ceil(i/2) */
        for (idx_t j = 0; j <= i; ++j) {
            seq_t sum = 0;
            for (idx_t l = start_l; l < i; ++l) {
                if (j <= l) sum += tri[l][j];
            }
            if (j == i) {
                seq_t val = (seq_t)i * 2 + 1;
                tri[i][j] = val - sum;
            } else {
                tri[i][j] = -sum;
            }
        }
    }
    next_row = r + 1;          /* all rows up to r are now ready */

    return tri[r][c];
}
