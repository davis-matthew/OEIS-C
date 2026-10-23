/*
 * A193848  Triangular array: (1/2)*A193846.
 * https://oeis.org/A193848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   flatten the triangular array produced by the recurrence a(r,k) =
 *            2·a(r‑1,k) + 4·a(r‑1,k‑1) – 2·a(r‑2,k‑1) – 3·a(r‑2,k‑2) with
 *            a(0,0)=1, a(1,0)=2, a(1,1)=4. The result a(n) is the n‑th entry
 *            in row‑major order. This implements OEIS A193848
 * Range:     u8 n <= 11, u16 n <= 39, u32 n <= 127, u64 n <= 449,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Integer square‑root for 64‑bit unsigned numbers. */
static idx_t floor_isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = 1ULL << 62;               /* largest power of four ≤ x */
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Cached triangular data.  triangle[r][k] holds a(r,k). */
static seq_t **triangle = NULL;         /* NULL indicates no data yet */
static size_t rows_allocated = 0;       /* number of rows currently stored */

/* Ensure the triangular array is built up to row `max_row`. */
static void ensure_triangle(size_t max_row)
{
    if (rows_allocated > 0 && max_row < rows_allocated) return;

    /* Grow the array of row pointers. */
    triangle = (seq_t **)realloc(triangle, (max_row + 1) * sizeof(seq_t *));
    if (triangle == NULL) abort(); /* out of memory */

    for (size_t r = rows_allocated; r <= max_row; ++r) {
        size_t len = r + 1;
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (row == NULL) abort(); /* out of memory */

        if (r == 0) {
            row[0] = (seq_t)1;
        } else if (r == 1) {
            row[0] = (seq_t)2;
            row[1] = (seq_t)4;
        } else {
            seq_t *prev1 = triangle[r - 1]; /* row r‑1, length r   */
            seq_t *prev2 = triangle[r - 2]; /* row r‑2, length r‑1 */
            for (size_t i = 0; i < len; ++i) row[i] = (seq_t)0;
            /* Add 2·a(r‑1,k) and 4·a(r‑1,k‑1) */
            for (size_t k = 0; k < r; ++k) {
                seq_t v = prev1[k];
                row[k]     += v * (seq_t)2;   /* 2*a(r-1,k)   */
                row[k + 1] += v * (seq_t)4;   /* 4*a(r-1,k+1) */
            }
            /* Subtract 2·a(r‑2,k‑1) and 3·a(r‑2,k‑2) */
            for (size_t k = 0; k < r - 1; ++k) {
                seq_t v = prev2[k];
                if (k + 1 < len)
                    row[k + 1] -= v * (seq_t)2;   /* 2*a(r-2,k+1) */
                if (k + 2 < len)
                    row[k + 2] -= v * (seq_t)3;   /* 3*a(r-2,k+2) */
            }
        }
        triangle[r] = row;
    }
    rows_allocated = max_row + 1;
}

/* Required function.  Computes the n‑th term of the flattened sequence. */
seq_t a(idx_t n)
{
    /* Determine the row that contains the n‑th term.  The index
       formula is floor((sqrt(1+8n)-1)/2). */
    idx_t sqrt_term = floor_isqrt_u64(1 + (idx_t)8 * n);
    size_t row = (size_t)((sqrt_term - (idx_t)1) / (idx_t)2);

    ensure_triangle(row);

    /* Compute the linear offset of the start of this row. */
    idx_t start = (idx_t)row * (row + 1) / 2;

    /* Column position within the row. */
    size_t col = (size_t)(n - start);

    return triangle[row][col];
}
