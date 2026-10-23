/*
 * A189601  Number of permutations of 1..n with displacements restricted to
 *          {-7,-6,-5,-4,-3,-1,0,2}.
 * https://oeis.org/A189601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of 1..n in which for every i (1 ≤
 *            i ≤ n) * the displacement i - p(i) belongs to the set {-7, -6,
 *            -5, -4, -3, -1, 0, 2}. * * This implementation computes the
 *            values by dynamic programming over a * 10‑bit sliding window
 *            that keeps track of which of the numbers * j ∈ [i...
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 59, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define MAX_MASK (1 << 10)          /* 10‑bit window: j = i-2 … i+7 */

static const int disp[8] = { -7, -6, -5, -4, -3, -1, 0, 2 };

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;           /* not defined, harmless      */
    }

    /* DP arrays: dp_curr holds counts for position i,
     * dp_next holds counts for position i+1.       */
    seq_t dp_curr[MAX_MASK];
    seq_t dp_next[MAX_MASK];

    /* initialise arrays to zero */
    for (unsigned int s = 0; s < MAX_MASK; ++s) {
        dp_curr[s] = 0;
        dp_next[s] = 0;
    }

    /* At i = 1, numbers ≤ 0 are considered already used.
     * j = i - 2 … i + 7 → numbers for i = 1: -1 … 8.
     * Bits 0 (j = -1) and 1 (j = 0) must be 1. */
    dp_curr[0b11] = 1;              /* init_mask = 3 */

    for (idx_t pos = 1; pos <= n; ++pos) {
        /* clear next container */
        for (unsigned int s = 0; s < MAX_MASK; ++s) {
            dp_next[s] = 0;
        }

        for (unsigned int mask = 0; mask < MAX_MASK; ++mask) {
            seq_t val = dp_curr[mask];
            if (!val) continue;

            /* try all allowed displacements */
            for (int i = 0; i < 8; ++i) {
                int d = disp[i];
                idx_t j = pos - (idx_t)d;          /* value at position pos */

                /* reject values outside 1..n */
                if (j < 1 || j > n) continue;

                /* Determine bit index for number j in the current window.
                 * At position pos, window covers [pos-2, pos+7].
                 * Bit index k = j - (pos-2) = pos - d - (pos-2) = 2 - d.  */
                unsigned int k = (unsigned int)(2 - d);   /* 0 <= k <= 9 */

                if (mask & (1u << k)) continue;          /* already used */

                unsigned int new_mask = mask | (1u << k);
                unsigned int next_mask = new_mask >> 1;    /* shift to next position */

                dp_next[next_mask] += val;
            }
        }

        /* advance to next position */
        for (unsigned int s = 0; s < MAX_MASK; ++s) {
            dp_curr[s] = dp_next[s];
        }
    }

    /* After processing all positions, the window is for i = n+1:
     * numbers n-1 and n map to bits 0 and 1 respectively.
     * Both must be present (i.e., bits 0 and 1 set). */
    seq_t result = 0;
    for (unsigned int mask = 0; mask < MAX_MASK; ++mask) {
        if ((mask & 0x3u) == 0x3u) {
            result += dp_curr[mask];
        }
    }
    return result;
}
