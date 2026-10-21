/*
 * A202390  Triangle T(n,k), read by rows, given by (1, 0, 1, 0, 0, 0, 0, 0,
 *          0, 0, ...) DELTA (1, 1, -1, 0, 0, 0, 0, 0, 0, 0, ...) where DELTA
 *          is the operator defined in A084938.
 * https://oeis.org/A202390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = 2*T(n-1,k) + T(n-1,k-1) + T(n-2,k-2) - T(n-2,k); a(n)
 *            is the triangle read by rows, with T(0,0)=T(1,0)=T(1,1)=1
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   index_to_rowk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/index_to_rowk.h"

static seq_t **TRIANGLE = NULL;          /* array of row pointers */
static size_t TRIANGLE_CAP = 0;          /* allocated capacity of TRIANGLE */
static size_t TRIANGLE_MAX = 0;          /* largest row index that has been computed */
static int TRIANGLE_INIT   = 0;          /* 0 = not initialized, 1 = initialized */

/* Ensure TRIANGLE has space for at least (need+1) rows. */
static void ensure_triangle_capacity(size_t need)
{
    if (need < TRIANGLE_CAP) return;
    size_t new_cap = TRIANGLE_CAP ? TRIANGLE_CAP * 2 : 1;
    if (new_cap < need + 1) new_cap = need + 1;
    TRIANGLE = (seq_t **)realloc(TRIANGLE, new_cap * sizeof(seq_t *));
    for (size_t i = TRIANGLE_CAP; i < new_cap; ++i) TRIANGLE[i] = NULL;
    TRIANGLE_CAP = new_cap;
}

/* Compute row r of the triangle.  Assumes rows r-1 and r-2 exist. */
static void compute_row(size_t r)
{
    seq_t *row = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    for (size_t k = 0; k <= r; ++k)
    {
        /* Base cases */
        if (r == 0 && k == 0) { row[k] = 1; continue; }
        if (r == 1 && (k == 0 || k == 1)) { row[k] = 1; continue; }

        seq_t a = 0, b = 0, c = 0, d = 0;
        if (r >= 1 && k <= r - 1) a = 2 * TRIANGLE[r - 1][k];
        if (k >= 1)                b = TRIANGLE[r - 1][k - 1];
        if (r >= 2 && k >= 2)      c = TRIANGLE[r - 2][k - 2];
        if (r >= 2 && k <= r - 2)  d = TRIANGLE[r - 2][k];
        row[k] = a + b + c - d;
    }
    TRIANGLE[r] = row;
}

/* Initialise the triangle with rows 0 and 1. */
static void init_triangle(void)
{
    if (TRIANGLE_INIT) return;
    ensure_triangle_capacity(2);
    compute_row(0);
    compute_row(1);
    TRIANGLE_MAX = 1;          /* rows 0 and 1 computed */
    TRIANGLE_INIT = 1;
}

/* Compute all rows required up to max_row (inclusive). */
static void ensure_triangle_upto(size_t max_row)
{
    if (max_row <= TRIANGLE_MAX) return;
    ensure_triangle_capacity(max_row + 1);
    for (size_t r = TRIANGLE_MAX + 1; r <= max_row; ++r)
        compute_row(r);
    TRIANGLE_MAX = max_row;
}

/* Public entry point for OEIS harness. */
seq_t a(idx_t n)
{
    /* Lazily initialise the triangle. */
    init_triangle();

    size_t row, k;
    index_to_rowk(n, &row, &k);
    ensure_triangle_upto(row);
    return TRIANGLE[row][k];
}
