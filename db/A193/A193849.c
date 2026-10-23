/*
 * A193849  Triangular array: (1/2)*A193847.
 * https://oeis.org/A193849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the element (r,k) of the triangular array T defined by
 *            T(0,0) = 1; T(r,0) = 3*T(r-1,0) + 1; T(r,k) = 2*T(r-1,k-1) +
 *            3*T(r-1,k) for 0 < k < r; T(r,r) = 2*T(r-1,r-1). The index n
 *            corresponds to the unique pair (r,k) such that n = r*(r+1)/2 + k
 * Range:     u8 n <= 10, u16 n <= 37, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine row r and offset of that row */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Base case: first row only */
    if (r == 0) {
        return (seq_t)1;
    }

    /* Allocate the previous row (row 0) */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t));
    if (!prev) {
        /* Allocation failed – return 0 as a safe fallback */
        return (seq_t)0;
    }
    prev[0] = (seq_t)1;

    /* Build rows up to r */
    for (idx_t row = 1; row <= r; ++row) {
        seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * (row + 1));
        if (!curr) {
            free(prev);
            return (seq_t)0;
        }

        /* First element of the row */
        curr[0] = (seq_t)3 * prev[0] + 1;

        /* Middle elements */
        for (idx_t col = 1; col <= row; ++col) {
            seq_t a_val = prev[col - 1];
            seq_t b_val = (col < row) ? prev[col] : (seq_t)0;
            curr[col] = a_val * 2 + b_val * 3;
        }

        free(prev);
        prev = curr;
    }

    /* Retrieve the requested element and free memory */
    seq_t result = prev[k];
    free(prev);
    return result;
}
