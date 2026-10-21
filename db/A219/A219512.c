/*
 * A219512  Triangle of third-order Eulerian numbers: 3-Stirling permutations
 *          enumerated by ascents.
 * https://oeis.org/A219512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   * a(n) is the entry T(r,k) of the triangle of third-order
 *            Eulerian numbers, * defined by * T(1,0) = 1 * T(n+1,k) =
 *            (k+1)*T(n,k) + (3*n - k + 1)*T(n,k-1) * where 0 ≤ k < n+1. * *
 *            For n ≥ 1 let r be the unique integer with * r*(r-1)/2 < n ≤
 *            r*(r+1)/2 (the row containing the n-th term). * Let * k =...
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *triangle = NULL;         /* linear storage of the triangle */
static idx_t   max_row = 0;            /* greatest row that has been computed */

/* Compute the integer r for a given n: smallest r with r(r+1)/2 >= n */
static idx_t
row_for_index(idx_t n)
{
    idx_t r = 1;
    /* Since n is at most a few thousand, a simple loop is fast enough. */
    while ((r * (r + 1)) / 2 < n)
        ++r;
    return r;
}

/* Store the triangle up to row 'needed_row' (inclusive).  The table is
 * enlarged with realloc if necessary.  The triangle is stored as a
 * linear array where index of T(r,k) is (r-1)*r/2 + k. */
static void
ensure_triangle(idx_t needed_row)
{
    if (needed_row <= max_row)
        return;                       /* nothing to do */

    /* Grow the storage if required. */
    if (triangle == NULL) {
        triangle = (seq_t *)malloc(sizeof(seq_t));
        if (triangle == NULL) return; /* out of memory :-(.  Caller will read garbage. */
        max_row = 0;
    }

    /* Allocate enough space for all rows up to needed_row. */
    size_t new_len = (size_t)(needed_row * (needed_row + 1) / 2);
    triangle = (seq_t *)realloc(triangle, new_len * sizeof(seq_t));
    if (triangle == NULL) return; /* out of memory :( */

    /* Initialise new entries progressively.  Row 1 is assumed to be already set. */
    if (max_row == 0) {           /* first time: set the very first row */
        triangle[0] = (seq_t)1;   /* T(1,0) = 1 */
        max_row = 1;
    }

    /* Compute rows from (max_row+1) up to needed_row. */
    for (idx_t r = max_row + 1; r <= needed_row; ++r) {
        /* previous row starts at (r-2)*(r-1)/2 and has length r-1 */
        idx_t prev_start = (r - 2) * (r - 1) / 2;
        idx_t prev_len   = r - 1;

        /* new row starts at (r-1)*r/2 */
        idx_t new_start = (r - 1) * r / 2;

        for (idx_t k = 0; k < r; ++k) {
            seq_t term1 = 0;
            if (k < prev_len) {
                /* (k+1) * T(r-1,k) */
                term1 = (seq_t)(k + 1) * triangle[prev_start + k];
            }

            seq_t term2 = 0;
            if (k > 0) {
                /* (3*(r-1) - k + 1) * T(r-1,k-1) */
                idx_t coeff = 3 * (r - 1) - k + 1;
                term2 = (seq_t)coeff * triangle[prev_start + (k - 1)];
            }

            triangle[new_start + k] = term1 + term2;
        }
    }

    max_row = needed_row;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* outside the offset, not used */

    /* Find the row r that contains the n-th term. */
    idx_t r = row_for_index(n);

    /* Expand the triangle if necessary. */
    ensure_triangle(r);

    /* Compute column index within the row. */
    idx_t prev_total = (r - 1) * r / 2;          /* total previous cells */
    idx_t k = n - prev_total - 1;                /* 0‑based column */

    /* Linear index of T(r,k). */
    idx_t pos = prev_total + k;
    return triangle[pos];
}
