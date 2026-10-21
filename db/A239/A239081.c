/*
 * A239081  Number of self-inverse permutations p on [n] with displacement of
 *          elements restricted by 9: |p(i)-i| <= 9.
 * https://oeis.org/A239081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of self‑inverse permutations p on [n] with
 *            |p(i)-i| <= 9 for all i (OEIS A239081)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a dynamic programming over a 10‑bit mask
   representing which of the next at most 9 positions have already
   been matched with an element < the current position.  The DP
   runs in O(10·n) time and uses O(2^9) memory.  */

#include <string.h>
#include <stdbool.h>

#define MASK_BITS 10
#define MASK_SIZE (1 << MASK_BITS)

/* External linkage function as required by the build. */
seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* DP arrays: dp[mask] – ways after processing current position;
       ndp[mask] – next position.  */
    static seq_t dp[MASK_SIZE];
    static seq_t ndp[MASK_SIZE];

    /* reset DP arrays for this call */
    memset(dp, 0, sizeof dp);
    dp[0] = (seq_t)1;

    /* Main loop over positions 1 .. n. */
    for (idx_t pos = 1; pos <= n; ++pos) {
        /* Number of remaining positions after current one. */
        idx_t rem = n - pos;
        int maxd = rem < 9 ? (int)rem : 9;

        /* reset ndp */
        memset(ndp, 0, sizeof ndp);

        /* iterate over all masks */
        for (int mask = 0; mask < MASK_SIZE; ++mask) {
            seq_t ways = dp[mask];
            if (!ways) {
                continue;
            }

            if (mask & 1) {          /* current position already matched */
                int newmask = mask >> 1;
                ndp[newmask] += ways;
            } else {                 /* current position free */
                /* option 1: leave as a fixed point */
                int newmask = mask >> 1;
                ndp[newmask] += ways;

                /* option 2: pair with a future position d (1..maxd) */
                for (int d = 1; d <= maxd; ++d) {
                    if (!(mask & (1 << d))) {
                        int newmask2 = (mask | (1 << d)) >> 1;
                        ndp[newmask2] += ways;
                    }
                }
            }
        }

        /* prepare for next iteration */
        memcpy(dp, ndp, sizeof dp);
    }

    /* After processing all positions, no pending matches should remain. */
    return dp[0];
}
