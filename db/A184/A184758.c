/*
 * A184758  Half the number of n X 6 binary arrays with no 1 having an
 *          adjacent 1 both above and to its left.
 * https://oeis.org/A184758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (number of n×6 binary arrays with no 1 having an adjacent
 *            1 both above and to its left) / 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined below offset */

    /* Precompute allowed transitions between rows */
    static uint64_t trans[64];
    static bool init = false;
    if (!init) {
        for (int p = 0; p < 64; ++p) {
            uint64_t mask = 0;
            for (int r = 0; r < 64; ++r) {
                bool ok = true;
                for (int j = 0; j < 6; ++j) {
                    if ((r >> j) & 1) {
                        if (((p >> j) & 1) && j > 0 && ((r >> (j - 1)) & 1)) {
                            ok = false;
                            break;
                        }
                    }
                }
                if (ok)
                    mask |= ((uint64_t)1 << r);
            }
            trans[p] = mask;
        }
        init = true;
    }

    /* DP over rows */
    seq_t dp[64];
    seq_t dp_new[64];
    for (int i = 0; i < 64; ++i)
        dp[i] = 1; /* first row: all 64 patterns are allowed */

    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < 64; ++i)
            dp_new[i] = 0; /* zero assign (works for all seq_t types) */

        for (int p = 0; p < 64; ++p) {
            uint64_t mask = trans[p];
            if (dp[p] == 0)
                continue;
            /* Iterate over all r such that mask has bit r set */
            for (int r = 0; r < 64; ++r) {
                if ((mask >> r) & 1ULL)
                    dp_new[r] += dp[p];
            }
        }

        for (int i = 0; i < 64; ++i)
            dp[i] = dp_new[i];
    }

    /* Sum all final row patterns */
    seq_t raw = 0;
    for (int i = 0; i < 64; ++i)
        raw += dp[i];

    /* Return half the count */
    return raw / (seq_t)2;
}
