/*
 * A200765  Number of 0..n arrays x(0..4) of 5 elements with nondecreasing
 *          average value.
 * https://oeis.org/A200765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 147, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: number of 0..n arrays x(0..4) of 5 elements with nondecreasing average value
   Implemented by dynamic programming over partial sums, using prefix‑sum optimisation. */

#include <stdlib.h>
#include <stddef.h>

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;          /* Undefined for n=0, return 0 for safety. */

    size_t N      = (size_t)n;
    size_t maxSum = 5 * N;                 /* Maximum possible total sum after 5 elements. */

    /* Allocate working arrays.  All are sized to maxSum+1 to avoid reallocation. */
    seq_t *dp_prev = (seq_t *)calloc(maxSum + 1, sizeof(seq_t));
    seq_t *dp_curr = (seq_t *)calloc(maxSum + 1, sizeof(seq_t));
    seq_t *prefix  = (seq_t *)calloc(maxSum + 1, sizeof(seq_t));

    if (!dp_prev || !dp_curr || !prefix)   /* If allocation fails, return zero. */
        return 0;

    /* Step 0: choose a0 freely from 0..N. */
    for (size_t s = 0; s <= N; ++s)
        dp_prev[s] = 1;

    /* Iterate i = 1..4 (building elements a1..a4). */
    for (size_t i = 1; i <= 4; ++i) {
        size_t currMax = i * N;           /* Max sum after i elements. */
        size_t nextMax = (i + 1) * N;     /* Max sum after (i+1) elements. */

        /* Prefix sums of dp_prev up to currMax. */
        prefix[0] = dp_prev[0];
        for (size_t k = 1; k <= currMax; ++k)
            prefix[k] = prefix[k - 1] + dp_prev[k];

        /* Compute dp_curr[t] for t = 0..nextMax. */
        for (size_t t = 0; t <= nextMax; ++t) {
            /* L = ceil(t / (i+1))  -- smallest a_i allowed. */
            size_t low  = (t + (i + 1) - 1) / (i + 1);
            /* H = min(N, t)       -- largest a_i allowed. */
            size_t high = (t <= N) ? t : N;

            if (high < low) {
                dp_curr[t] = 0;
                continue;
            }

            size_t k_start = t - high;     /* Corresponds to t - a_i = largest sum subtract. */
            size_t k_end   = t - low;      /* Corresponds to smallest sum subtract. */

            seq_t val = prefix[k_end];
            if (k_start > 0)
                val -= prefix[k_start - 1];
            dp_curr[t] = val;
        }

        /* Prepare for next iteration: swap dp_prev and dp_curr. */
        seq_t *tmp = dp_prev;
        dp_prev    = dp_curr;
        dp_curr    = tmp;
        /* dp_curr will be overwritten in the next loop iteration. */
    }

    /* Sum all entries of dp_prev to obtain a(n). */
    seq_t ans = 0;
    for (size_t s = 0; s <= maxSum; ++s)
        ans += dp_prev[s];

    /* Clean up. */
    free(dp_prev);
    free(dp_curr);
    free(prefix);

    return ans;
}
