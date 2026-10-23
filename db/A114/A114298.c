/*
 * A114298  First row of Modified Schroeder numbers for q=7 (A114294).
 * https://oeis.org/A114298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of lattice paths from (0,0) to (n,n) with steps
 *            (1,0),(0,1),(1,1) staying between the lines y = x and y = 3x/4.
 *            This is computed by dynamic programming: for each column i (0 ≤
 *            i ≤ n) we keep the counts for all feasible y‑coordinates j ≥
 *            ceil(3i/4) and j ≤ i. The recurrence for a cel...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate two rows of DP.  calloc gives zeroed storage, which works
       for builtin numeric types and for big‑integer types that have a
       zero value at bit‑wise zero.  The cast is required for C++. */
    seq_t *prev = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!prev || !next) {
        free(prev);
        free(next);
        return 0;          /* allocation failed */
    }

    prev[0] = 1;           /* a(0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        /* lower bound of allowed y at this column */
        idx_t j_min      = (3 * i + 3) / 4;          /* ceil(3*i/4) */
        idx_t j_min_prev = (3 * (i - 1) + 3) / 4;    /* ceil(3*(i-1)/4) */

        for (idx_t j = j_min; j <= i; ++j) {
            seq_t val = 0;

            /* from left: (i-1, j) */
            if (j <= i - 1 && j >= j_min_prev)
                val += prev[j];

            /* from below: (i, j-1)  – already computed in this row */
            if (j > j_min)
                val += next[j - 1];

            /* from diagonal: (i-1, j-1) */
            if (j - 1 <= i - 1 && j - 1 >= j_min_prev)
                val += prev[j - 1];

            next[j] = val;
        }

        /* swap rows for next iteration */
        seq_t *tmp  = prev;
        prev        = next;
        next        = tmp;
    }

    seq_t result = prev[n];

    free(prev);
    free(next);
    return result;
}
