/*
 * A193822  Mirror of the triangle A193821.
 * https://oeis.org/A193822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = w(r, r-k) where r = floor((sqrt(8*n + 1) - 1)/2), k = n -
 *            r*(r+1)/2 (the column in the r‑th row). The values w(r, j) are
 *            defined by the triangle A193821: w(0,0) = 1 w(r,0) = 2^(r-1) (r
 *            > 0) w(r,r) = 3^(r-1) (r > 0) w(r,j) = w(r-1,j-1) + 2*w(r-1,j)
 *            (1 ≤ j ≤ r-1) The sequence A193822 is obta...
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>

/* Static cache for the triangle w(r,j) */
static seq_t *triangle = NULL;           /* contiguous array to store rows */
static size_t triangle_capacity = 0;     /* allocated capacity of 'triangle' */
static idx_t triangle_max_row = 0;       /* highest row currently computed */

/* Ensure that rows up to 'r' are stored in the cache */
static void ensure_triangle_upto_row(idx_t r)
{
    /* Initial allocation and first row */
    if (triangle == NULL) {
        triangle_capacity = 32;
        triangle = (seq_t *)calloc(triangle_capacity, sizeof(seq_t));
        if (triangle == NULL) return;    /* allocation failed – give up */
        triangle[0] = (seq_t)1;          /* w(0,0) */
        triangle_max_row = 0;
    }
    if (r <= triangle_max_row) return;    /* nothing to do */

    /* Compute the total number of elements needed up to row r */
    idx_t needed = (r + 1) * (r + 2) / 2;
    size_t needed_cap = (size_t)needed;
    if (triangle_capacity < needed_cap) {
        size_t new_cap = triangle_capacity ? triangle_capacity : 32;
        while (new_cap < needed_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(triangle, new_cap * sizeof(seq_t));
        if (tmp == NULL) return;          /* allocation failed – give up */
        triangle = tmp;
        triangle_capacity = new_cap;
    }

    /* Compute rows from (triangle_max_row + 1) up to r */
    for (idx_t row = triangle_max_row + 1; row <= r; ++row) {
        idx_t offset = row * (row + 1) / 2;    /* start index of this row */

        if (row == 0) {                       /* already set */
            triangle[offset] = (seq_t)1;
            continue;
        }

        /* Left boundary: w(row,0) = 2^(row-1) */
        seq_t left = (seq_t)1;
        for (idx_t i = 1; i < row; ++i) left *= (seq_t)2;
        triangle[offset] = left;

        /* Right boundary: w(row,row) = 3^(row-1) */
        seq_t right = (seq_t)1;
        for (idx_t i = 1; i < row; ++i) right *= (seq_t)3;
        triangle[offset + row] = right;

        /* Interior cells: w(row,j) = w(row-1,j-1) + 2*w(row-1,j) */
        idx_t prev_offset = offset - row;     /* start of previous row */
        for (idx_t j = 1; j < row; ++j) {
            seq_t a = triangle[prev_offset + j - 1];
            seq_t b = triangle[prev_offset + j];
            triangle[offset + j] = a + (b << 1);
        }
    }

    triangle_max_row = r;
}

seq_t a(idx_t n)
{
    /* Find the row 'r' containing n */
    double d = sqrt((double)8 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    /* Correct possible rounding errors */
    if (r > 0) {
        while (((idx_t)(r + 1) * (r + 2) / 2) <= n) ++r;
    }
    while ((r * (r + 1)) / 2 > n) --r;

    /* Compute column in the row and mirrored column */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;          /* column index in the row (0-based) */
    idx_t j = r - k;                /* column after mirroring (0-based) */

    ensure_triangle_upto_row(r);
    return triangle[offset + j];
}
