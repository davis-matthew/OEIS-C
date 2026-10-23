/*
 * A186492  Recursive triangle for calculating A186491.
 * https://oeis.org/A186492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, changed
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive triangle A186492:
   a(n) is the n-th entry in row‑major order of the triangle T(n,k) defined by
   T(0,0) = 1
   T(n,k) = (2*k-1)*T(n-1,k-1) + (2*k+2)*T(n-1,k+1)
   for 0 ≤ k ≤ n, missing terms treated as zero. */

#include <stdint.h>
#include <stdlib.h>

/* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
   For the problem constraints r is small, so a simple loop suffices. */
static uint64_t row_of(uint64_t n)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    return r;
}

/* Return the a(n) value in the triangle. */
seq_t a(idx_t n)
{
    /* Base case. */
    if (n == 0) return (seq_t)1;

    uint64_t N  = (uint64_t)n;             /* work in uint64_t */
    uint64_t row = row_of(N);              /* row index */
    uint64_t start = row * (row + 1) / 2;  /* first linear index of this row */
    uint64_t col  = N - start;             /* column index in this row */

    /* Allocate two rows of size row+1. */
    seq_t *prev = (seq_t *)malloc((row + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((row + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        free(prev); free(curr);
        return (seq_t)0; /* memory allocation failure */
    }

    /* Initialize base row (row 0). */
    for (uint64_t i = 0; i <= row; ++i) {
        prev[i] = curr[i] = (seq_t)0;
    }
    prev[0] = (seq_t)1;

    /* Build rows 1 … row incrementally. */
    for (uint64_t r = 1; r <= row; ++r) {
        /* k = 0 element */
        curr[0] = (seq_t)2 * prev[1];

        /* 1 ≤ k ≤ r-1 elements */
        for (uint64_t k = 1; k < r; ++k) {
            seq_t left  = ((seq_t)(2 * k - 1)) * prev[k - 1];
            seq_t right = ((seq_t)(2 * k + 2)) * prev[k + 1];
            curr[k] = left + right;
        }

        /* k = r element */
        curr[r] = ((seq_t)(2 * r - 1)) * prev[r - 1];

        /* Prepare for next iteration: swap prev and curr. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[col];

    /* Clean up. */
    free(prev);
    free(curr);

    return result;
}
