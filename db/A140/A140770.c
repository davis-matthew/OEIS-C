/*
 * A140770  3D analog of A081113: the number of (n-1)-step paths a 3D chess
 *          king can make starting from one face of the n X n X n cube to the
 *          opposite one.
 * https://oeis.org/A140770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (vᵀ A^{n‑1} v)², where A is the n×n tridiagonal matrix
 *            with 1’s on the main, sub‑ and super‑diagonals
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of n‑1‑step 3‑D king paths from one face of an n×n×n cube to the opposite face. */
#include <stdlib.h>

seq_t a(idx_t n) {
    size_t sz = (size_t)n;
    if (sz == 0) return (seq_t)0;

    seq_t *cur = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!cur || !nxt) {
        free(cur);
        free(nxt);
        return (seq_t)0;   /* allocation failure */
    }

    /* initial counts: one way from each start cell to itself (0 steps) */
    for (size_t i = 0; i < sz; ++i) {
        cur[i] = (seq_t)1;
    }

    for (idx_t step = 1; step < n; ++step) {
        if (sz >= 2) {
            /* left boundary (j = 0) */
            nxt[0] = cur[0] + cur[1];

            /* interior cells (1 ≤ j ≤ sz-2) */
            for (size_t j = 1; j + 1 < sz; ++j) {
                nxt[j] = cur[j-1] + cur[j] + cur[j+1];
            }

            /* right boundary (j = sz-1) */
            nxt[sz-1] = cur[sz-2] + cur[sz-1];
        } else {
            /* sz == 1: only one cell, always stay */
            nxt[0] = cur[0];
        }

        /* swap arrays for next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* total number of paths from any cell on one side to any cell on the opposite side */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < sz; ++i) {
        total += cur[i];
    }

    seq_t result = total * total;   /* square the count */

    free(cur);
    free(nxt);
    return result;
}
