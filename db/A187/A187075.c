/*
 * A187075  A Galton triangle: T(n,k) = 2*k*T(n-1,k) + (2*k-1)*T(n-1,k-1).
 * https://oeis.org/A187075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,c) where r = smallest integer with r(r+1)/2 >= n and
 *            c = n - (r-1)r/2. T(n,k) satisfies: T(1,1)=1,
 *            T(n,k)=2*k*T(n-1,k)+(2*k-1)*T(n-1,k-1)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 51, u64 n <= 143,
 *            u128 n <= 368, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n <= 0) return (seq_t)0;          /* out of range, return 0 */
    /* Find r such that r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t tri = 1; /* r(r+1)/2 */
    while (tri < n) {
        ++r;
        tri += r;
    }
    /* c = n - (r-1)r/2 */
    idx_t prev_tri = tri - r;
    idx_t c = n - prev_tri;   /* 1 <= c <= r */

    if (r == 1) return (seq_t)1;   /* only cell (1,1) */

    /* Dynamic programming: keep two rows */
    /* Allocate arrays size (r+1) to use 1-based indexing */
    seq_t *prev = (seq_t*)calloc((size_t)(r + 1), sizeof(seq_t));
    seq_t *curr = (seq_t*)calloc((size_t)(r + 1), sizeof(seq_t));
    if (!prev || !curr) { /* allocation failure, return 0 */
        free(prev); free(curr);
        return (seq_t)0;
    }

    prev[1] = (seq_t)1;   /* T(1,1) */

    for (idx_t row = 2; row <= r; ++row) {
        for (idx_t k = 1; k <= row; ++k) {
            seq_t two_k = (seq_t)2 * (seq_t)k;
            seq_t term1 = two_k * prev[k];
            seq_t term2 = (two_k - (seq_t)1) * prev[k - 1];
            curr[k] = term1 + term2;
        }
        /* Prepare for next iteration */
        /* swap prev and curr pointers */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[c];

    free(prev);
    free(curr);
    return result;
}
