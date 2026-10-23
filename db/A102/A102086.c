/*
 * A102086  Triangular matrix, read by rows, that satisfies: T(n,k) =
 *          [T^2](n-1,k) when n>k>=0, with T(n,n) = (n+1).
 * https://oeis.org/A102086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A102086
 * Recurrence:  T(0,0)         = 1
 *              T(n,n)         = n + 1
 *              T(n,k)         = Σ_{m=k}^{n-1} T(n-1,m) * T(m,k)   for n > k ≥ 0
 * The sequence a(n) is read row‑wise from this triangular array:
 *    a(n) = T(row,col)
 *    row = largest r with r(r+1)/2 ≤ n
 *    col = n – r(r+1)/2
 */

#include <stdlib.h>

/* external integer types supplied by the build */
seq_t a(idx_t n);

static seq_t **tri = NULL;          /* tri[r][k] = T(r,k) */
static idx_t rows_allocated = 0;    /* number of rows stored (max index + 1) */

/* build all rows up to target_row (inclusive) */
static void ensure_rows(idx_t target_row)
{
    if (target_row < rows_allocated)
        return;

    /* reallocate row pointer array */
    tri = (seq_t**)realloc(tri, sizeof(seq_t*) * (target_row + 1));
    if (!tri)
        return;  /* crash on OOM – acceptable for test harness */

    /* allocate memory for each new row */
    for (idx_t r = rows_allocated; r <= target_row; ++r) {
        tri[r] = (seq_t*)malloc(sizeof(seq_t) * (r + 1));
        if (!tri[r])
            return;  /* crash on OOM – acceptable for test harness */
    }

    /* compute the new rows */
    for (idx_t r = rows_allocated; r <= target_row; ++r) {
        if (r == 0) {
            tri[0][0] = (seq_t)1;
        } else {
            /* inner entries */
            for (idx_t k = 0; k < r; ++k) {
                seq_t sum = 0;
                for (idx_t m = k; m < r; ++m) {
                    sum += tri[r - 1][m] * tri[m][k];
                }
                tri[r][k] = sum;
            }
            /* diagonal entry */
            tri[r][r] = (seq_t)(r + 1);
        }
    }

    rows_allocated = target_row + 1;
}

seq_t a(idx_t n)
{
    /* locate row and column of the n‑th element */
    idx_t row = 0;
    while ((row + 1) * (row + 2) / 2 <= n)
        ++row;
    idx_t col = n - row * (row + 1) / 2;

    /* build all rows needed */
    ensure_rows(row);

    return tri[row][col];
}
