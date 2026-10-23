/*
 * A035618  Number of partitions of n into parts 3k and 3k+1 with at least one
 *          part of each type.
 * https://oeis.org/A035618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 63, u32 n <= 188, u64 n <= 615,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035618: number of partitions of n into parts 3k and 3k+1
   with at least one part of each type.  The implementation uses a
   dynamic 1‑D DP array where the four states are:
        0 – no 3k part used,  no 3k+1 part used
        1 – a 3k part used,   no 3k+1 part used
        2 – no 3k part used,  a 3k+1 part used
        3 – both types used */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* return value type is seq_t; n is idx_t (unsigned 64‑bit) */
    /* allocated state array: dp[sum * 4 + state] */
    static idx_t cache_max = 0;      /* not used directly but kept for clarity */
    static seq_t *dp = NULL;
    static size_t dp_cap = 0;        /* number of seq_t entries allocated */

    /* ensure we have enough space for indices 0..n and 4 states each */
    size_t needed = (size_t)(n + 1) * 4;
    if (needed > dp_cap) {
        seq_t *newdp;
#ifdef __cplusplus
        newdp = new seq_t[needed];
#else
        newdp = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!newdp) exit(EXIT_FAILURE);
#endif
        /* zero the new array */
        for (size_t i = 0; i < needed; ++i) newdp[i] = 0;
        /* copy any previously stored data (up to old cap) */
        if (dp) {
            size_t old = dp_cap;
            for (size_t i = 0; i < old; ++i) newdp[i] = dp[i];
#ifdef __cplusplus
            delete[] dp;
#else
            free(dp);
#endif
        }
        dp     = newdp;
        dp_cap = needed;
    }

    /* reset the table up to n (all states) */
    for (size_t i = 0; i <= (size_t)n * 4; ++i) dp[i] = 0;
    dp[0] = 1;   /* start with empty partition (state 0) */

    /* add parts of type 3k (mask 1) */
    for (idx_t part = 3; part <= n; part += 3) {
        for (idx_t sum = part; sum <= n; ++sum) {
            size_t cur  = (size_t)sum * 4;
            size_t prev = (size_t)(sum - part) * 4;
            /* state transition: s -> s | 1 */
            dp[cur + 1] += dp[prev + 0];
            dp[cur + 1] += dp[prev + 1];
            dp[cur + 3] += dp[prev + 2];
            dp[cur + 3] += dp[prev + 3];
        }
    }

    /* add parts of type 3k+1 (mask 2) */
    for (idx_t part = 1; part <= n; part += 3) {
        for (idx_t sum = part; sum <= n; ++sum) {
            size_t cur  = (size_t)sum * 4;
            size_t prev = (size_t)(sum - part) * 4;
            /* state transition: s -> s | 2 */
            dp[cur + 2] += dp[prev + 0];
            dp[cur + 3] += dp[prev + 1];
            dp[cur + 2] += dp[prev + 2];
            dp[cur + 3] += dp[prev + 3];
        }
    }

    return dp[(size_t)n * 4 + 3];  /* state 3 = both types used */
}
