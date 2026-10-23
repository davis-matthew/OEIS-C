/*
 * A025870  Expansion of 1/((1-x^4)*(1-x^7)*(1-x^11)).
 * https://oeis.org/A025870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 4, 7, and 11.
   Computed by dynamic programming (table of size n+1). */
#include <stdlib.h>
#include <stdint.h>   /* for uint64_t */

static seq_t *dp = NULL;   /* array dp[0..max_n] */
static size_t dp_cap = 0;  /* allocated capacity */

/* Ensure capacity for indices up to n (inclusive). */
static void ensure_capacity(size_t n) {
    size_t needed = n + 1;
    if (needed <= dp_cap) return;

    seq_t *newdp;
#ifdef __cplusplus
    newdp = new seq_t[needed];
#else
    newdp = (seq_t*)malloc(needed * sizeof(seq_t));
#endif
    if (dp) {
#ifdef __cplusplus
        delete[] dp;
#else
        free(dp);
#endif
    }
    dp = newdp;
    dp_cap = needed;
    /* initialize all elements to zero */
    for (size_t i = 0; i < dp_cap; ++i) dp[i] = 0;
}

seq_t a(idx_t n) {
    /* allocate sufficient storage */
    ensure_capacity((size_t)n);

    /* recompute the DP table for the current n */
    dp[0] = 1;
    for (size_t i = 1; i <= (size_t)n; ++i) dp[i] = 0;

    static const idx_t parts[3] = {4, 7, 11};
    for (size_t pidx = 0; pidx < 3; ++pidx) {
        size_t p = (size_t)parts[pidx];
        for (size_t i = p; i <= (size_t)n; ++i) {
            dp[i] += dp[i - p];
        }
    }
    return dp[(size_t)n];
}
