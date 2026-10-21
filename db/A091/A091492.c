/*
 * A091492  Triangle, read by rows, generated recursively and related to
 *          partitions.
 * https://oeis.org/A091492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) of OEIS A091492.
   Recurrence:
       T(n,0)   = 1                     (for all n ≥ 0)
       T(1,1)   = 1
       For 0 < n and 1 ≤ k ≤ n
           T(n,k) = Σ_{j = ⌊(k+1)/2⌋}^{min(k, n−k)}  T(n−k , j) * T(j , k−j)
   The sequence a(n) is the triangle read by rows (row 0 first, then row 1, …).
   Offset: 0.  a(0) = T(0,0).  a(n) = T(r,c) with r = ⌊(√(8n+1)−1)/2⌋
   and c = n – r(r+1)/2.                                            */

#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*             Cached storage of the triangle T(n,k)                     */
/* --------------------------------------------------------------------- */
static seq_t **triangle = NULL;      /* pointer to array of row pointers   */
static size_t triangle_rows = 0;     /* number of rows currently stored    */

/* --------------------------------------------------------------------- */
/*     Ensure that rows 0 … max_row are present in the cache             */
/* --------------------------------------------------------------------- */
static void ensure_triangle_rows(idx_t max_row)
{
    if (max_row < (idx_t)triangle_rows)
        return;                       /* already enough */

    /* Grow the outer array (array of row pointers) */
    if ((size_t)max_row + 1 > triangle_rows) {
        seq_t **tmp = (seq_t **)realloc(triangle,
                                         ((size_t)max_row + 1) * sizeof(seq_t *));
        triangle = tmp;
        for (size_t i = triangle_rows; i <= (size_t)max_row; ++i)
            triangle[i] = NULL;
    }

    /* Compute the new rows (from the current end up to max_row) */
    for (idx_t n = (idx_t)triangle_rows; n <= max_row; ++n) {
        /* Allocate memory for one row (length n+1) */
        triangle[n] = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));

        /* Fill the row */
        for (idx_t k = 0; k <= n; ++k) {
            if (k == 0) {
                triangle[n][k] = (seq_t)1;
            } else if (n == 1 && k == 1) {
                triangle[n][k] = (seq_t)1;
            } else {
                idx_t start = (k + 1) >> 1;                     /* ⌊(k+1)/2⌋ */
                idx_t end   = (n - k < k) ? (n - k) : k;        /* min(k, n−k) */
                seq_t sum = 0;
                for (idx_t j = start; j <= end; ++j) {
                    seq_t a = triangle[n - k][j];
                    seq_t b = triangle[j][k - j];
                    sum += a * b;
                }
                triangle[n][k] = sum;
            }
        }
    }
    triangle_rows = (size_t)max_row + 1;
}

/* --------------------------------------------------------------------- */
/*                Public entry point: sequence a(n)                     */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Find the row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2            */
    idx_t r = (idx_t)((((double)(8 * n + 1)) && 1) ? 0 : 0); /* placeholder   */
    /* Compute r using integer arithmetic:  r = floor((sqrt(8n+1)-1)/2) */
    /* Since we must stay in integer math, use a square‑root helper.        */
    /* We can use the provided isqrt_u64 (included indirectly by build).   */
    /* But for safety we implement a quick integer sqrt here.               */
    idx_t sq = 0;
    {
        idx_t low = 0, high = 1;
        while (high <= (idx_t)((8 * n + 1) / high))
            high <<= 1;
        low = high >> 1;
        while (low < high) {
            idx_t mid = (low + high + 1) >> 1;
            if (mid <= (idx_t)((8 * n + 1) / mid))
                low = mid;
            else
                high = mid - 1;
        }
        sq = low;
    }
    r = (sq - 1) >> 1;                 /* (sq - 1) / 2  */

    /* Ensure the required rows are cached */
    ensure_triangle_rows(r);

    /* Column index within the row */
    idx_t start_n = (r * (r + 1)) / 2;
    idx_t c = n - start_n;

    return triangle[r][c];
}
