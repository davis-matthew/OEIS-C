/*
 * A200884  Number of 0..6 arrays x(0..n+1) of n+2 elements without any
 *          interior element greater than both neighbors.
 * https://oeis.org/A200884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of arrays x(0..n+1) of length n+2 with values 0..6
   where no interior element is greater than both neighbors.
   Computed by dynamic programming over the last value and whether
   the previous step was an up step.  The algorithm runs in
   O(n) time and O(1) memory (constant 14 big‑int cells). */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Sequence is defined only for n ≥ 1.  For invalid indices return 0. */
    if (n < 1)
        return 0;

    /* Length of the array is n+2; we need to process n+1 steps
       after placing the first element. */
    size_t steps = (size_t)n + 1;

    /* dp[v][u] – number of partial sequences whose last value is v
       (0..6) and whose previous transition was an up step
       (u = 1) or not (u = 0).  7 * 2 = 14 entries. */
    seq_t dp[7][2];
    seq_t ndp[7][2];

    /* Initialise: only the first element is chosen; no previous step
       exists, so we treat it as “not up”. */
    for (int v = 0; v < 7; ++v) {
        dp[v][0] = 0;  /* clear first element counts */
        dp[v][1] = 0;
        if (v < 7) {
            dp[v][0] = 1;   /* one way to start with value v */
        }
    }

    /* Iterate through the remaining positions. */
    for (size_t step = 0; step < steps; ++step) {
        /* Reset ndp to zero. */
        for (int i = 0; i < 7; ++i)
            for (int j = 0; j < 2; ++j)
                ndp[i][j] = 0;

        /* Transition from each state (v1, up1) to all possible next
           values v2.  If the previous step was an up, we cannot go
           strictly down – i.e., we require v2 >= v1. */
        for (int v1 = 0; v1 < 7; ++v1) {
            /* Previous step was not up: any next value is allowed. */
            seq_t val0 = dp[v1][0];
            if (val0) {
                for (int v2 = 0; v2 < 7; ++v2) {
                    int up2 = (v2 > v1) ? 1 : 0;
                    ndp[v2][up2] += val0;
                }
            }

            /* Previous step was an up: next value must be >= v1. */
            seq_t val1 = dp[v1][1];
            if (val1) {
                for (int v2 = v1; v2 < 7; ++v2) {
                    int up2 = (v2 > v1) ? 1 : 0;
                    ndp[v2][up2] += val1;
                }
            }
        }

        /* Copy ndp into dp for the next iteration. */
        for (int i = 0; i < 7; ++i)
            for (int j = 0; j < 2; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum over all final states to get the total number of sequences. */
    seq_t total = 0;
    for (int v = 0; v < 7; ++v) {
        total += dp[v][0];
        total += dp[v][1];
    }

    return total;
}
