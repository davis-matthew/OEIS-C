/*
 * A188497  a(n) = A188493(n+1) - A188491(n) - A188495(n).
 * https://oeis.org/A188497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of permutations p of [n] with * abs(p(i)-i) <= 3
 *            for all i, * p(1) <= 2, p(2) <= 3, and p(4) >= 2 (for n >= 2). *
 *            * The sequence is computed by a dynamic programme over a sliding
 *            * window of width 7. Each state is a 7‑bit mask telling which of
 *            the * positions j in [i-3,i+3] are alre...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>              /* for NULL */
#include <stdint.h>              /* for uintptr_t */

#define K 3
#define WIN (2*K+1)              /* Window width = 7 */
#define MASK_W (1<<WIN)          /* 128 states */

seq_t a(idx_t n)
{
    /* a(0) and a(1) are 0 by definition */
    if (n < 2)
        return (seq_t)0;

    /* DP arrays.  The type seq_t may be any unsigned integer type
     * (including arbitrary‑precision), so we initialise them in a
     * loop. */
    seq_t dp[MASK_W];
    seq_t dp_next[MASK_W];
    for (unsigned i = 0; i < MASK_W; ++i) {
        dp[i] = 0;
        dp_next[i] = 0;
    }

    /* Build initial mask for i = 1.
       Bits set to 1 for indices out of range [1..n]. */
    unsigned initMask = 0;
    for (unsigned off = 0; off < WIN; ++off) {
        idx_t idx = 1 - K + off;          /* right index at i=1 */
        if (idx < 1 || idx > n)
            initMask |= (1u << off);
    }
    dp[initMask] = 1;

    seq_t ans = 0;

    for (idx_t i = 1; i <= n; ++i) {
        /* Clear next DP except when i==n (final step). */
        if (i != n) {
            for (unsigned m = 0; m < MASK_W; ++m)
                dp_next[m] = 0;
        }

        for (unsigned mask = 0; mask < MASK_W; ++mask) {
            seq_t cur = dp[mask];
            if (cur == 0)
                continue;

            /* Try all possible right indices within the window. */
            for (unsigned off = 0; off < WIN; ++off) {
                if (mask & (1u << off))
                    /* already used or out of range */
                    continue;

                idx_t j = i - K + off;    /* actual right index */

                /* Special constraints for positions 1,2,4. */
                if (i == 1 && j > 2)
                    continue;
                if (i == 2 && j > 3)
                    continue;
                if (i == 4 && j < 2)
                    continue;

                unsigned newmask = mask | (1u << off);

                if (i == n) {
                    /* Last position: add to answer. */
                    ans += cur;
                } else {
                    /* Shift window to the right for next i. */
                    unsigned shiftMask = newmask >> 1;
                    idx_t nextRight = i + K + 1;   /* first new index entering window */
                    unsigned newbit = (nextRight > n) ? 1u : 0u;
                    shiftMask |= (newbit << (WIN-1));
                    dp_next[shiftMask] += cur;
                }
            }
        }

        if (i != n) {
            /* Move to next iteration. */
            for (unsigned m = 0; m < MASK_W; ++m)
                dp[m] = dp_next[m];
        }
    }

    return ans;
}
