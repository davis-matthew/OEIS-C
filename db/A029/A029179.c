/*
 * A029179  Expansion of 1/((1-x^2)*(1-x^4)*(1-x^6)*(1-x^9)).
 * https://oeis.org/A029179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 2, 4, 6 and 9.
   Computed by dynamic programming on the coin‑change recurrence.
   dp[n] = dp[n‑2] + dp[n‑4] + dp[n‑6] + dp[n‑9]   (with dp[0] = 1). */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *dp  = NULL;        /* memoised values up to dp_size-1 */
    static idx_t dp_size = 0;        /* number of entries currently stored */
    static const idx_t parts[4] = {2,4,6,9};

    /* If the requested n is beyond the current memoised range, extend it. */
    if (n + 1 > dp_size) {
        idx_t new_size = n + 1;                     /* n is small (≤~10000) */
        seq_t *tmp = (seq_t*)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                  /* allocation failed */
        dp = tmp;

        /* Initialise the array and compute all values up to the new size. */
        dp[0] = (seq_t)1;
        for (idx_t i = 1; i < new_size; ++i) dp[i] = (seq_t)0;

        for (int pi = 0; pi < 4; ++pi) {
            idx_t p = parts[pi];
            for (idx_t i = p; i < new_size; ++i) {
                dp[i] += dp[i - p];
            }
        }
        dp_size = new_size;
    }

    return dp[n];
}
