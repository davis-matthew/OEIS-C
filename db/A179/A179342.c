/*
 * A179342  Number of permutations of 1..n with i-6 <= p(i) <= i+4.
 * https://oeis.org/A179342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count permutations of 1..n with i-6 <= p(i) <= i+4
   using a dynamic programming on a sliding window of width 11. */
#include <stdint.h>
#include <stdbool.h>

#define WINDOW 11
#define MASKSIZE (1 << WINDOW)

seq_t a(idx_t n) {
    /* Special case: nothing to do if n == 0 (not in OEIS index) */
    if (n == 0) return 0;

    /* Two DP arrays of size 2048 (1<<11) */
    static seq_t dp[2][MASKSIZE];
    static bool initialized = false;
    if (!initialized) {
        for (int m = 0; m < MASKSIZE; ++m) {
            dp[0][m] = 0;
            dp[1][m] = 0;
        }
        initialized = true;
    }

    /* Initial mask for i = 1 */
    int initMask = 0;
    for (int t = 0; t < WINDOW; ++t) {
        idx_t j = (idx_t)1 - 6 + (idx_t)t;          /* j = i-6 + t for i = 1 */
        if (j < 1 || j > n) initMask |= (1 << t);   /* invalid positions pre-matched */
    }

    int cur  = 0, nxt = 1;
    for (int m = 0; m < MASKSIZE; ++m) dp[cur][m] = 0;
    dp[cur][initMask] = 1;

    /* DP over i = 1 .. n */
    for (idx_t i = 1; i <= n; ++i) {
        /* zero next array */
        for (int m = 0; m < MASKSIZE; ++m) dp[nxt][m] = 0;

        /* bit to set when a new position j = i+5 enters the window */
        int new_bit10 = ((i + 5) <= n) ? 0 : 1;

        for (int mask = 0; mask < MASKSIZE; ++mask) {
            seq_t ways = dp[cur][mask];
            if (ways == 0) continue;

            int bit0 = mask & 1;   /* j = i-6 */

            for (int t = 0; t < WINDOW; ++t) {
                idx_t j = i - 6 + t;
                if (j < 1 || j > n) continue;          /* out of valid range */
                if (mask & (1 << t)) continue;         /* already matched */

                /* If j = i-6 is still unmatched we must match it now */
                if (!bit0 && t != 0) continue;

                int newMask = mask | (1 << t);          /* mark it matched */
                newMask = (newMask >> 1) & (MASKSIZE - 1); /* shift window */
                newMask |= (new_bit10 << 10);          /* new entry at the end */

                dp[nxt][newMask] += ways;
            }
        }

        /* swap arrays for next iteration */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* All positions matched => mask with all bits set */
    return dp[cur][MASKSIZE - 1];
}
