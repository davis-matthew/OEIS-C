/*
 * A092082  Triangle of numbers related to triangle A092083; generalization of
 *          Stirling numbers of second kind A008277, Lah-numbers A008297, ...
 * https://oeis.org/A092082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   compute_row_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A092082:  a(n) is the n‑th element of the triangle A092083 read
 *  row‑wise.  The triangle satisfies
 *
 *    a(1,1) = 1,          a(n,0) = 0,         a(n,m) = 0 if n < m
 *    a(n+1,m) = (6*n + m) * a(n,m) + a(n,m-1)   (n >= m >= 1)
 *
 *  The function below returns a(n) for all n ≥ 1.  It keeps a cache
 *  of computed rows; rows are generated only on demand.  The cache
 *  uses a 1‑based indexing scheme so that triangle[row][col] holds
 *  a(row, col).  The function is thread‑local and works under C11
 *  and C++17.  No external libraries are required.  Memory usage is
 *  O(r²) where r ≈ √(2n). */

#include <stdlib.h>   /* malloc, free, calloc, realloc */
#include "../utils/compute_row_col.h"

static seq_t **triangle = NULL;   /* triangle[row] is a pointer to its row (1‑based) */
static idx_t   max_row   = 0;     /* highest row that has been computed */

/* Allocate or grow the cache so that rows up to 'up_to' exist.          */
static void ensure_rows(idx_t up_to)
{
    if (triangle == NULL) {
        /* initialize with the first row: row 1 */
        triangle = (seq_t **)calloc(2, sizeof(seq_t*));  /* indices 0,1 */
        seq_t *row1 = (seq_t *)malloc(2 * sizeof(seq_t));
        row1[0] = 0;        /* unused */
        row1[1] = 1;        /* a(1,1) = 1 */
        triangle[1] = row1;
        max_row = 1;
        return;
    }

    while (max_row < up_to) {
        idx_t curr = max_row + 1;          /* new row number          */
        seq_t *prev = triangle[max_row];   /* previous row            */
        seq_t *row  = (seq_t *)malloc((curr + 1) * sizeof(seq_t));
        row[0] = 0;                        /* unused 0‑th column     */

        for (idx_t m = 1; m <= curr; ++m) {
            seq_t prev_val  = (m <= max_row) ? prev[m] : 0;
            seq_t prev_pm1  = (m - 1 <= max_row) ? prev[m-1] : 0;
            seq_t coeff     = (seq_t)6 * (seq_t)max_row + (seq_t)m;
            row[m]          = coeff * prev_val + prev_pm1;
        }

        triangle = (seq_t **)realloc(triangle, (curr + 1) * sizeof(seq_t*));
        triangle[curr] = row;
        ++max_row;
    }
}

/* Public entry point requested by the framework.                               */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* for safety; the sequence is defined for n >= 1 */

    idx_t r, c;
    compute_row_col(n, &r, &c);
    ensure_rows(r);
    return triangle[r][c];
}
