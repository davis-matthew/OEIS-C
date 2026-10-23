/*
 * A152406  Triangle T, read by rows, where column k of T = column 0 of matrix
 *          power T^(k+1) for k>0, with column 0 of T = column 1 of T^2
 *          (shifted).
 * https://oeis.org/A152406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where T is the triangle defined by (1) T(0,0)
 *            = 1. (2) For n > 0, k > 0: T(n,k) = Σ_{j=0}^{n-k} T(n-k,j) *
 *            T(j+k-1,k-1). (3) For all n ≥ 0: T(n,0) = Σ_{j=1}^{n} T(n,j) *
 *            T(j,1). The sequence is returned by flattening the triangle
 *            row‑by‑row. Each call is memoised so that any or...
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   idx_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/idx_at.h"

static seq_t *triangle = NULL;          /* flattened triangle */
static size_t  triangle_cap = 0;        /* allocated capacity in elements   */
static size_t  triangle_len  = 0;       /* number of filled elements         */
static idx_t   rows_done     = 0;       /* number of fully computed rows     */

/* compute (row, col) from linear index n */
static inline void index_to_row_col(idx_t n, idx_t *row, idx_t *col)
{
    /* r = floor((sqrt(8*n+1)-1)/2) */
    double r_d = (sqrt((double)(8.0 * (double)n + 1.0)) - 1.0) * 0.5;
    idx_t r = (idx_t)r_d;
    *row = r;
    *col = n - r * (r + 1) / 2;
}

/* ensure that rows 0..row_needed are fully computed in triangle */
static void ensure_row(idx_t row_needed)
{
    while (rows_done <= row_needed) {
        idx_t row = rows_done;
        idx_t row_off = triangle_len;   /* starting index of this row */

        /* Resize storage if necessary */
        size_t needed = triangle_len + (row + 1);
        if (needed > triangle_cap) {
            size_t new_cap = triangle_cap ? triangle_cap * 2 : 128;
            while (new_cap < needed)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(triangle, new_cap * sizeof(seq_t));
            triangle = tmp;
            triangle_cap = new_cap;
        }

        /* Compute row 0: T(0,0)=1 */
        if (row == 0) {
            triangle[0] = (seq_t)1;
        } else {
            /* Compute columns k = 1 .. row using the recurrence */
            for (idx_t k = 1; k <= row; ++k) {
                seq_t sum = 0;
                idx_t maxj = row - k;
                for (idx_t j = 0; j <= maxj; ++j) {
                    seq_t a = triangle[idx_at(row - k, j)];
                    seq_t b = triangle[idx_at(j + k - 1, k - 1)];
                    sum += a * b;
                }
                triangle[row_off + k] = sum;
            }
            /* Compute column 0: T(row,0) */
            seq_t sum0 = 0;
            for (idx_t j = 1; j <= row; ++j) {
                seq_t a = triangle[row_off + j];
                seq_t b = triangle[idx_at(j, 1)];
                sum0 += a * b;
            }
            triangle[row_off] = sum0;
        }

        triangle_len += (row + 1);
        rows_done = row + 1;
    }
}

seq_t a(idx_t n)
{
    idx_t row, col;
    index_to_row_col(n, &row, &col);
    ensure_row(row);
    return triangle[idx_at(row, col)];
}
