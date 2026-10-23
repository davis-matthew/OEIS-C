/*
 * A101950  Product of A049310 and A007318 as lower triangular matrices.
 * https://oeis.org/A101950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is obtained from the lower triangular matrix T defined by
 *            T(0,0)=1 and for n≥1, k∈[0..n]
 *            T(n,k)=T(n‑1,k‑1)+T(n‑1,k)−T(n‑2,k) (with out‑of‑bounds terms
 *            treated as 0). The OEIS sequence a(n) is the row‑major
 *            flattening of T: a(n)=T(r,k) where r,r+1 satisfy
 *            r(r+1)/2≤n<(r+1)(r+2)/2 and k=n−r(r+1)/2
 * Range:     i8 n <= 81, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Helpers:   tri64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/tri64.h"

/* Static storage for the computed rows of the triangle. */
static seq_t **triangle = NULL;
static idx_t triangle_rows = 0;   /* number of rows currently stored   */
static idx_t triangle_cap  = 0;   /* allocated capacity of the array   */

/* Ensure that all rows up to 'up_to' have been computed. */
static void ensure_triangle(idx_t up_to)
{
    if (triangle_rows > up_to) return;

    /* Grow the outer array if needed. */
    if (triangle_cap == 0) triangle_cap = 8;
    while (triangle_cap <= up_to) triangle_cap <<= 1;
    if (triangle_cap != triangle_rows) {
        triangle = (seq_t**)realloc(triangle,
                                    triangle_cap * sizeof(seq_t*));
        if (!triangle) abort(); /* allocation failure */
    }

    /* Row 0 is the base case. */
    if (triangle_rows == 0) {
        triangle[0] = (seq_t*)malloc(sizeof(seq_t));
        if (!triangle[0]) abort();
        triangle[0][0] = 1;
        triangle_rows = 1;
    }

    /* Compute successive rows from triangle_rows up to up_to. */
    for (idx_t n = triangle_rows; n <= up_to; ++n) {
        seq_t *row = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!row) abort();
        triangle[n] = row;

        for (idx_t k = 0; k <= n; ++k) {
            seq_t val = 0;
            if (k <= n - 1)          val += triangle[n - 1][k];
            if (k > 0)               val += triangle[n - 1][k - 1];
            if (n >= 2 && k <= n - 2) val -= triangle[n - 2][k];
            row[k] = val;
        }
    }

    triangle_rows = up_to + 1;
}

/* Return the nth term of sequence A101950. */
seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    long double t = sqrt((long double)8 * n + 1);
    idx_t r = (idx_t)((t - 1.0L) / 2.0L);

    /* Adjust for possible rounding errors. */
    while ((__int128)(r + 1) * (r + 2) / 2 <= n) ++r;
    while ((__int128)r * (r + 1) / 2 > n) --r;

    /* Column within the row. */
    idx_t k = n - (idx_t)tri64(r);

    ensure_triangle(r);
    return triangle[r][k];
}
