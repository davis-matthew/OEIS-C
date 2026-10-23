/*
 * A153811  A recursive triangular sequence with row sums (5^(n - 1)*(n +
 *          3)!)/12: A(n,k)= A(n - 1, k - 1) + A(n - 1, k) + 5 *(2 + n) (13 +
 *          5* n)*A(n - 2, k - 1).
 * https://oeis.org/A153811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 24, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A153811 – recursive triangular sequence with row sums (5^(n-1)(n+3)!)/12
 * Recurrence (for n ≥ 4):
 *     A(n,k) = A(n-1,k-1) + A(n-1,k) + 5*(2+n)*(13+5*n)*A(n-2,k-1)
 * Initial rows (explicitly stored):
 *     Row 1:          {2}
 *     Row 2:          {25, 25}
 *     Row 3:          {2, 1496, 2}
 * The sequence a(n) is the concatenation of the triangular rows,
 *   reading row 1, then row 2, etc.  Index n starts at 1.
 *
 * The implementation keeps a static cache of computed rows up to the
 *   highest required row and returns the element corresponding to
 *   the linear index n.
 */

#include <stddef.h>          /* size_t */
#include <stdbool.h>         /* bool (required for compatibility) */
#include "../utils/seq_alloc_array.h"  /* seq_alloc_array */

/* Maximum number of rows needed for n up to ~10000 (ceiling solution) */
#define MAX_ROWS 200

/* Static cache of triangular rows; index 1..MAX_ROWS. */
static seq_t *tri[MAX_ROWS + 1] = {0};
static size_t max_cached_row = 0;

/* Compute and cache triangular rows up to 'row'. */
static void ensure_rows_up_to(size_t row)
{
    if (row <= max_cached_row) return;

    for (size_t r = max_cached_row + 1; r <= row; ++r) {
        /* Allocate row of length r (1-indexed row number). */
        tri[r] = seq_alloc_array(r);  /* seq_alloc_array(size_t n) returns seq_t* */

        /* Base rows 1..3 are hard‑coded. */
        if (r == 1) {
            tri[1][0] = (seq_t)2;
        } else if (r == 2) {
            tri[2][0] = (seq_t)25;
            tri[2][1] = (seq_t)25;
        } else if (r == 3) {
            tri[3][0] = (seq_t)2;
            tri[3][1] = (seq_t)1496;
            tri[3][2] = (seq_t)2;
        } else {
            /* Recurrence for rows r ≥ 4. */
            const seq_t coeff_factor = (seq_t)5 * (seq_t)(2 + r) * (seq_t)(13 + 5 * r);

            for (size_t k = 0; k < r; ++k) {
                seq_t t1 = (k > 0)     ? tri[r - 1][k - 1] : (seq_t)0;
                seq_t t2 = (k < r - 1) ? tri[r - 1][k]     : (seq_t)0;
                seq_t t3 = (k > 0 && k < r - 1)
                    ? coeff_factor * tri[r - 2][k - 1]
                    : (seq_t)0;
                tri[r][k] = t1 + t2 + t3;
            }
        }
    }
    max_cached_row = row;
}

/* Main function exposed by the program. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n <= 0, defensive */

    /* Determine the row r such that offset(r) <= n <= offset(r+1)-1.
       offset(r) = r*(r-1)/2 + 1. */
    size_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* Ensure rows up to r are computed. */
    ensure_rows_up_to(r);

    /* Linear position within the row (0‑based). */
    idx_t offset = (idx_t)(r * (r - 1) / 2 + 1);
    size_t pos0 = (size_t)(n - offset); /* pos0 ∈ [0, r-1] */

    return tri[r][pos0];
}
