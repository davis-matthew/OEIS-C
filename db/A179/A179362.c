/*
 * A179362  Number of permutations of 1..n with i-10<=p(i)<=i+2.
 * https://oeis.org/A179362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A179362: Number of permutations of 1..n with i-10 ≤ p(i) ≤ i+2.
   Implementation uses a sliding‑window DP over a 13‑bit state.
   For each position i we keep track of which of the 13 candidate values
   (i-10 … i+2) are still unused.  The transition removes the chosen value,
   shifts the window by one, and adds the new value i+3 if it is ≤ n.
   Complexity: O(n·2¹³) time, O(2¹³) memory. */

#include <stdint.h>
#include <string.h>

#define WINDOW 13                       /* 13 possible values per position */
#define STATE_MASK ((1U << WINDOW) - 1) /* 0 .. 8191 */

seq_t a(idx_t n) {
    static seq_t dp[STATE_MASK + 1];
    static seq_t ndp[STATE_MASK + 1];
    memset(dp, 0, sizeof(dp));
    memset(ndp, 0, sizeof(ndp));

    /* initial window for i = 1: values j in [1,3] (since n≥1) */
    uint16_t init_mask = 0;
    for (idx_t j = 1; j <= n && j <= 3; ++j) {
        uint16_t k = (uint16_t)(j + 9); /* j+9 = j - (1-10) */
        init_mask |= (1U << k);
    }
    dp[init_mask] = 1;

    for (idx_t i = 1; i <= n; ++i) {
        for (uint16_t mask = 0; mask <= STATE_MASK; ++mask) {
            seq_t ways = dp[mask];
            if (!ways) continue;
            for (uint16_t k = 0; k < WINDOW; ++k) {
                if (!(mask & (1U << k))) continue;
                /* remove chosen value */
                uint16_t new_mask = (uint16_t)((mask & ~(1U << k)) >> 1);
                /* add new value i+3 if it exists */
                if (i + 3 <= n) {
                    new_mask |= (1U << 12);
                }
                ndp[new_mask] += ways;
            }
        }
        /* swap dp and ndp, clearing ndp for next iteration */
        for (uint16_t mask = 0; mask <= STATE_MASK; ++mask) {
            dp[mask] = ndp[mask];
            ndp[mask] = 0;
        }
    }
    return dp[0];
}
