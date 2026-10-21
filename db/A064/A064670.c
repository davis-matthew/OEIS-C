/*
 * A064670  Triangle T(n,k) (1 <= k <= n) where the first column (T(n,1)) is
 *          the sequence of secant numbers A000364.
 * https://oeis.org/A064670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Recurrence for triangle T(n,k)
 *   T(1,1) = 1
 *   T(n+1,m) = m*m * Σ_{k=m-1..n} T(n,k)   (T(n,0) = 0)
 *
 * Sequence a(n) is the row‑wise enumeration of T:
 *   a(n) = T(row, col)
 *   row = ceil((sqrt(8*n+1)-1)/2)
 *   col = n - row*(row-1)/2
 *
 * The implementation builds the triangle lazily up to the requested row.
 * All arithmetic uses type seq_t, so it works for 8/16/32/64‑bit,
 * unsigned __int128, or arbitrary‑precision builds.
 */

#include <stdlib.h>
#include <math.h>

/* Static storage for the triangle: triangle[r][k] with 1‑based indices */
static seq_t **triangle = NULL;   /* pointer array, index 0 unused */
static idx_t triangle_rows = 0;   /* highest row currently computed */

/* Ensure rows 1..row are computed; reseize storage if needed. */
static void ensure_triangle_up_to(idx_t row)
{
    if (triangle_rows >= row)
        return;   /* already available */

    /* Expand row pointer array */
    triangle = (seq_t **)realloc(triangle,
                                 (row + 1) * sizeof(seq_t *));
    if (triangle == NULL)
        return; /* out‑of‑memory; caller will get 0 */

    /* Build missing rows */
    for (idx_t r = triangle_rows + 1; r <= row; ++r)
    {
        /* Allocate current row (indices 0..r; index 0 unused) */
        seq_t *cur = (seq_t *)malloc((r + 1) * sizeof(seq_t));
        if (cur == NULL)
            return; /* allocation failure */

        cur[0] = (seq_t)0;

        if (r == 1)
        {
            cur[1] = (seq_t)1;   /* T(1,1) = 1 */
        }
        else
        {
            seq_t *prev = triangle[r - 1];
            idx_t prev_len = r - 1;

            /* Build cumulative sums of previous row from the end. */
            seq_t *cum = (seq_t *)malloc((prev_len + 1) * sizeof(seq_t));
            if (cum == NULL)
            {
                free(cur);
                return; /* allocation failure */
            }
            seq_t s = (seq_t)0;
            for (idx_t k = prev_len; k >= 1; --k)
            {
                s += prev[k];
                cum[k] = s;
            }
            cum[0] = s;   /* total sum of previous row */

            /* Compute current row using the recurrence. */
            for (idx_t m = 1; m <= r; ++m)
            {
                seq_t sum = cum[m - 1];          /* Σ from m-1 to end */
                seq_t mm  = (seq_t)m;
                cur[m] = mm * mm * sum;
            }
            free(cum);
        }

        triangle[r] = cur;
    }
    triangle_rows = row;
}

/* Main entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n <= 0 */

    /* Find the row containing the n-th entry. */
    long double d = sqrtl((long double)8.0L * (long double)n + 1.0L);
    idx_t row = (idx_t)((d - 1.0L) / 2.0L);   /* floor of root formula */

    /* Adjust if the triangular number of row is smaller than n. */
    unsigned __int128 tri = (unsigned __int128)row * (row + 1u) / 2u;
    if (tri < n)
        ++row;

    /* Ensure the triangle has been built up to this row. */
    ensure_triangle_up_to(row);
    if (triangle == NULL || triangle[row] == NULL)
        return (seq_t)0; /* allocation failure */

    /* Column within the row. */
    idx_t col = n - (row * (row - 1u)) / 2u;

    return triangle[row][col];
}
