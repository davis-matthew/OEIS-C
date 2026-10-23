/*
 * A029375  Expansion of 1/((1-x^4)*(1-x^9)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A029375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 4, 9, 10, and 12
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (coin‑change DP, order of parts: 4, 9, 10, 12) */

#include <stdlib.h>

/* Static cache for computed values */
static seq_t *dp = NULL;       /* array of partition counts */
static idx_t dp_size = 0;      /* number of entries in dp (dp_size==0 means uninitialised) */

/* Ensure dp is computed up to index N (inclusive) */
static void compute_until(idx_t N)
{
    const idx_t parts[4] = {4, 9, 10, 12};
    const int num_parts = 4;

    /* If already computed up to N, nothing to do */
    if (dp_size > N) {
        return;
    }

    /* Allocate (or reallocate) the array and initialise it */
    dp = (seq_t *)realloc(dp, (N + 1) * sizeof(seq_t));
    /* Reinitialise all entries to 0 to avoid artefacts from previous computations */
    for (idx_t i = 0; i <= N; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;            /* one way to partition 0 */

    /* Classic coin‑change DP: for each part, add contributions */
    for (int p = 0; p < num_parts; ++p) {
        idx_t part = parts[p];
        for (idx_t i = part; i <= N; ++i) {
            dp[i] += dp[i - part];
        }
    }

    dp_size = N + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    compute_until(n);
    return dp[n];
}
