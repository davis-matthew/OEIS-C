/*
 * A210239  Triangle, read by rows, given by (2, -1, 0, 0, 0, 0, 0, 0, 0, ...)
 *          DELTA (2, -1/2, -1/2, 0, 0, 0, 0, 0, 0, 0, ...) where DELTA is the
 *          operator defined in A084938.
 * https://oeis.org/A210239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,c) where r,c are the row and column of n in the
 *            triangle read by rows, and T is defined by the recurrence
 *            T(0,0)=1, T(1,0)=T(1,1)=2,
 *            T(n,k)=T(n-1,k)+T(n-1,k-1)+T(n-2,k-1)+T(n-2,k-2) (k>=0, n>=2),
 *            with T(n,k)=0 if k<0 or k>n. See OEIS A210239
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   row_offset
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/row_offset.h"

/* Helper to compute the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static idx_t
compute_row(idx_t n)
{
    long double t = sqrtl((long double)8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((t - 1.0L) / 2.0L);
    /* adjust for rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    while (r * (r + 1) / 2 > n) r--;
    return r;
}

/* Dynamic table of the triangle values */
static seq_t *triangle = NULL;     /* 1-D array T(r,k) */
static size_t triangle_cap = 0;    /* allocated elements */
static idx_t  max_row_done = (idx_t)-1; /* largest completed row */

/* Ensure the triangle contains rows 0..r (inclusive) */
static void
ensure_rows(idx_t r)
{
    if (max_row_done == (idx_t)-1) {
        /* initialize empty table */
        triangle = (seq_t *)realloc(triangle, sizeof(seq_t));
        triangle_cap = 1;
        max_row_done = 0;
        triangle[0] = (seq_t)1; /* T(0,0) */
    }
    if (r <= max_row_done) return;

    /* Allocate storage for all needed rows */
    size_t new_size = (size_t)(r + 1) * (size_t)(r + 2) / 2;
    if (new_size > triangle_cap) {
        triangle = (seq_t *)realloc(triangle, new_size * sizeof(seq_t));
        triangle_cap = new_size;
    }

    /* Build rows from (max_row_done+1) up to r */
    for (idx_t cur = max_row_done + 1; cur <= r; ++cur) {
        idx_t offset_cur = row_offset(cur);
        if (cur == 0) {
            triangle[0] = (seq_t)1;
        } else if (cur == 1) {
            triangle[offset_cur + 0] = (seq_t)2;
            triangle[offset_cur + 1] = (seq_t)2;
        } else {
            idx_t offset_prev  = offset_cur - cur;        /* row cur-1 */
            idx_t offset_prev2 = offset_prev - (cur - 1); /* row cur-2 */
            for (idx_t k = 0; k <= cur; ++k) {
                seq_t v = 0;
                if (k <= cur - 1) v += triangle[offset_prev + k];
                if (k > 0)       v += triangle[offset_prev + k - 1];
                if (cur >= 2) {
                    if (k > 0 && k <= cur - 1) v += triangle[offset_prev2 + k - 1];
                    if (k >= 2)                v += triangle[offset_prev2 + k - 2];
                }
                triangle[offset_cur + k] = v;
            }
        }
    }
    max_row_done = r;
}

seq_t
a(idx_t n)
{
    idx_t r = compute_row(n);
    idx_t c = n - row_offset(r);
    ensure_rows(r);
    return triangle[row_offset(r) + c];
}
