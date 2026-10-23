/*
 * A134141  Generalized unsigned Stirling1 triangle, S1p(7).
 * https://oeis.org/A134141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n, m) = (6*m + n - 1)*a(n-1, m) + a(n-1, m-1), * a(1,1)=1. The
 *            function a(idx_t n) returns the nth element of * the triangle
 *            enumerated row by row (row 1 first, then row 2 left to right, …)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

static seq_t **triangle = NULL;   /* triangle[i][m], 1 ≤ m ≤ i */
static idx_t max_row = 0;

/* Ensure the triangle is computed up to the given row. */
static void ensure_triangle_up_to(idx_t r)
{
    idx_t i, m;

    if (r <= max_row)
        return;

    /* Allocate / resize the array of row pointers. */
    if (triangle == NULL) {
        triangle = (seq_t **)malloc(sizeof(seq_t *) * (r + 1));
        /* triangle[0] is unused. */
    } else {
        triangle = (seq_t **)realloc(triangle, sizeof(seq_t *) * (r + 1));
    }

    for (i = max_row + 1; i <= r; ++i) {
        /* Allocate the current row: indices 0..i (0 unused). */
        seq_t *row = alloc_seq_array((size_t)(i + 1));
        row[0] = (seq_t)0; /* unused, keep zero for safety */

        if (i == 1) {
            row[1] = (seq_t)1; /* base case a(1,1)=1 */
            triangle[i] = row;
            continue;
        }

        /* Compute all entries in the row using the recurrence. */
        for (m = 1; m <= i; ++m) {
            seq_t prev  = (m <= i - 1) ? triangle[i - 1][m] : (seq_t)0;
            seq_t prev2 = triangle[i - 1][m - 1];
            seq_t coeff = (seq_t)(6 * (idx_t)m + i - 1);
            row[m] = coeff * prev + prev2;
        }

        triangle[i] = row;
    }

    max_row = r;
}

/* Public function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for offset 0, just return 0 */

    /* Find the row r such that T_{r-1} < n ≤ T_r, where T_k = k*(k+1)/2. */
    idx_t r = 0;
    idx_t t = 0; /* current triangular number T_r */
    while (t < n) {
        ++r;
        t += r;
    }

    idx_t prev_t = t - r;            /* T_{r-1} */
    idx_t m = n - prev_t;            /* column index, 1-based */

    ensure_triangle_up_to(r);
    return triangle[r][m];
}
