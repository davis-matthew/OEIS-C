/*
 * A238312  Square row sums of the table A072233 (A008284).
 * https://oeis.org/A238312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} p(n,k)², where p(n,k) is the number of
 *            partitions of n into k positive parts (A072233)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 51, u64 n <= 144, u128 n <= 450,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#if defined(__cplusplus)
#include <cstddef>
#endif

seq_t a(idx_t n)
{
    /* Base case: a(0) = 1 */
    if (n == 0) return (seq_t)1;

    size_t N = (size_t)n;                /* use size_t for array indices */

#if defined(__cplusplus)
    /* Allocate two arrays of seq_t for two consecutive rows of the DP table */
    seq_t *row_prev = new seq_t[N + 1]();   /* value‑initialized to 0 */
    seq_t *row_curr = new seq_t[N + 1]();
#else
    /* For plain C, use malloc and manually zero the arrays */
    seq_t *row_prev = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *row_curr = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!row_prev || !row_curr) {          /* allocation failed */
        if (row_prev) free(row_prev);
        if (row_curr) free(row_curr);
        return (seq_t)0;
    }
    for (size_t i = 0; i <= N; ++i) {
        row_prev[i] = 0;
        row_curr[i] = 0;
    }
#endif

    row_prev[0] = 1;  /* p(0,0) = 1, all other p(0,k>0) = 0 is already 0 */

    seq_t result = 0;

    /* For each possible number of parts k = 1 .. N compute p(k, i) for all i ≤ N */
    for (size_t k = 1; k <= N; ++k) {
        /* Zero out the first k entries of the current row before use */
        for (size_t i = 0; i < k; ++i) row_curr[i] = 0;

        /* Compute row_curr[i] = p(i,k) using the recurrence:
           p(i,k) = p(i-k,k) + p(i-1,k-1)
           Here row_prev holds p(·,k-1) and row_curr holds progressively computed p(·,k). */
        for (size_t i = k; i <= N; ++i) {
            seq_t val_left   = row_prev[i - 1];     /* p(i-1,k-1) */
            seq_t val_right  = row_curr[i - k];     /* p(i-k,k)  */
            row_curr[i] = val_left + val_right;
        }

        /* Accumulate the square of p(N,k) into the final answer */
        seq_t pk = row_curr[N];
        result += pk * pk;

        /* Prepare for next k: swap the row pointers */
        seq_t *tmp = row_prev;
        row_prev = row_curr;
        row_curr = tmp;
    }

    /* Free the allocated memory */
#if defined(__cplusplus)
    delete[] row_prev;
    delete[] row_curr;
#else
    free(row_prev);
    free(row_curr);
#endif

    return result;
}
