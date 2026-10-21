/*
 * A201136  Number of n X 2 0..5 arrays with every row and column
 *          nondecreasing rightwards and downwards, and the number of
 *          instances of each value within one of each other.
 * https://oeis.org/A201136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 183, u64 n <= 183,
 *            u128 n <= 183, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A201136: number of n×2 arrays with entries 0..5, rows & columns
   nondecreasing, and the counts of each value differ by at most one.
   The term is computed by summing over all admissible count‑distributions.
   For a fixed distribution the number of valid arrays equals the number
   of ways to split the multiset into two columns so that, when the
   columns are sorted, column 1 never exceeds column 2 at any prefix.
   This is counted by a small DP on the six values with cumulative
   constraints. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset is 1 */

    /* Basic count per value and how many values receive the extra one */
    const idx_t k = n / 3;              /* floor(n/3)   */
    const idx_t r = (2 * n) % 6;        /* number of values with count k+1 */

    /* For performance, allocate buffers once */
    const size_t N = (size_t)n;
    seq_t *dp_prev = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *dp_curr = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *diff    = (seq_t*)malloc((N + 2) * sizeof(seq_t));  /* +1 for diff end slot */

    seq_t result = 0;
    const uint64_t subset_limit = 1ULL << 6;          /* 64 subsets of {0..5} */

    /* Iterate over all subsets of size r (exactly the required number of
       values with the larger count) */
    for (uint64_t mask = 0; mask < subset_limit; ++mask) {
        if (__builtin_popcount((unsigned)mask) != r) continue;

        /* Build the total count array c[v] for this subset */
        uint8_t c[6];
        for (int v = 0; v < 6; ++v)
            c[v] = (uint8_t)(k + ((mask >> v) & 1U));

        /* Reset DP buffers */
        memset(dp_prev, 0, (N + 1) * sizeof(seq_t));
        memset(dp_curr, 0, (N + 1) * sizeof(seq_t));
        dp_prev[0] = 1;                   /* start with empty prefix */

        idx_t prefix_total = 0;

        /* Process each of the six values */
        for (int v = 0; v < 6; ++v) {
            prefix_total += c[v];
            idx_t max_prefix = prefix_total / 2;   /* floor(total_v/2) */

            /* Difference array for range updates */
            memset(diff, 0, (N + 2) * sizeof(seq_t));

            /* For each achievable prefix sum in dp_prev,
               add contributions to all allowable new prefix sums. */
            idx_t max_p = (prefix_total <= (idx_t)N) ? prefix_total : (idx_t)N;
            for (idx_t p = 0; p <= max_p && p <= N; ++p) {
                seq_t val = dp_prev[p];
                if (val == 0) continue;

                idx_t max_split = c[v];
                if (max_split > (max_prefix - p))
                    max_split = max_prefix - p;
                if ((intptr_t)max_split < 0) continue;   /* no valid split */

                size_t start = (size_t)p;
                size_t end   = (size_t)(p + max_split);   /* inclusive */
                diff[start] += val;
                if (end + 1 <= N) diff[end + 1] -= val;
            }

            /* Prefix‑sum over diff to obtain dp_curr */
            seq_t running = 0;
            for (size_t np = 0; np <= N; ++np) {
                running += diff[np];
                dp_curr[np] = running;
            }

            /* Prepare for next value: dp_prev <- dp_curr */
            seq_t *tmp   = dp_prev;
            dp_prev      = dp_curr;
            dp_curr      = tmp;
            /* dp_curr will be overwritten in the next iteration */
        }

        /* dp_prev[n] holds the number of valid column splits for this
           distribution. Add it to the final result. */
        result += dp_prev[n];
    }

    /* Free temporary arrays */
    free(dp_prev);
    free(dp_curr);
    free(diff);

    return result;
}
