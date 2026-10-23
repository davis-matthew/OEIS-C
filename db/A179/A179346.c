/*
 * A179346  Number of permutations of 1..n with i-7<=p(i)<=i+4.
 * https://oeis.org/A179346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations p of 1..n such that i-7 ≤ p(i) ≤
 *            i+4. The value is obtained by a DP with a sliding 12‑bit window;
 *            at each step the leftmost unused value must be taken (the
 *            “leftmost” rule) to guarantee that every allowed value is
 *            eventually chosen
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Width of the sliding window (i+4) - (i-7) + 1 = 12 */
#define W      12
#define MASKS  (1u << W)

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* indexing starts at 1 */

    /* DP tables: only 4096 entries are needed (2^12) */
    static seq_t dp[MASKS];
    static seq_t dp_next[MASKS];
    seq_t *cur  = dp;
    seq_t *next = dp_next;

    /* reset tables */
    memset(cur, 0, sizeof(dp));

    /* Initial mask for position i = 1: mark values outside [1,n] as used */
    uint32_t init_mask = 0;
    for (uint32_t t = 0; t < W; ++t) {
        int64_t j = (int64_t)1 - 7 + t;      /* j = i-7+t  (i = 1) */
        if (j < 1 || j > (int64_t)n)
            init_mask |= (1u << t);
    }
    cur[init_mask] = 1;

    /* Process positions i = 1 .. n */
    for (idx_t i = 1; i <= n; ++i) {
        memset(next, 0, sizeof(dp_next));

        for (uint32_t mask = 0; mask < MASKS; ++mask) {
            seq_t ways = cur[mask];
            if (ways == 0) continue;

            /* If the leftmost value in the window is unused,
               it must be chosen now. */
            bool left_idle = ((mask & 1u) == 0);

            for (uint32_t t = 0; t < W; ++t) {
                int64_t j = (int64_t)i - 7 + t;   /* candidate value */
                if (j < 1 || j > (int64_t)n) continue;        /* out of range */
                if (mask & (1u << t)) continue;                /* already used */
                if (left_idle && t != 0) continue;             /* would discard unused leftmost */

                uint32_t mask2 = mask | (1u << t);    /* record use of j */
                uint32_t next_mask = mask2 >> 1;     /* shift window right */

                /* The new rightmost value is i+5; mark it used if it exceeds n. */
                if (i + (idx_t)5 > n)
                    next_mask |= (1u << (W - 1));

                next[next_mask] += ways;
            }
        }

        /* Advance to next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Build a bitmask of all values that must appear in the final window */
    uint32_t need_mask = 0;
    idx_t start_val = (n > 6) ? n - 6 : 1;          /* max(1, n-6) */
    for (idx_t j = start_val; j <= n; ++j) {
        int k = (int)(j - (n - 6));
        if (k >= 0 && k < (int)W)
            need_mask |= (1u << k);
    }

    /* Sum all DP states that contain all required values */
    seq_t result = 0;
    for (uint32_t mask = 0; mask < MASKS; ++mask) {
        if ((mask & need_mask) == need_mask)
            result += cur[mask];
    }
    return result;
}
