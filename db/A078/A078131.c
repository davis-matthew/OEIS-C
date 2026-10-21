/*
 * A078131  Numbers which can be written as sum of cubes > 1.
 * https://oeis.org/A078131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer (starting with n = 1) that can be
 *            written as a sum of cubes of integers ≥ 2. For n ≥ 72, a(n) = n
 *            + 83
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* idx_t and seq_t are defined by the test harness. */
seq_t a(idx_t n)
{
    /* First 71 terms are computed once via dynamic programming
       over the cubic coins 8, 27, 64, 125.  The maximal term we
       need to evaluate is a(71) < 155, so we search up to 154. */
    static bool initialized = false;
    static seq_t first71[71];

    if (!initialized) {
        const idx_t COINS[4] = {8, 27, 64, 125};

        const idx_t MAX_VAL = 154;          /* a(71) < 155 */
        const size_t NEED = 71;

        /* Boolean array for subset-sum DP. */
        bool *dp = (bool *)calloc(MAX_VAL + 1, sizeof(bool));
        dp[0] = true;

        for (idx_t i = 0; i < (idx_t)4; ++i) {
            idx_t c = COINS[i];
            for (idx_t v = c; v <= MAX_VAL; ++v) {
                if (dp[v - c]) {
                    dp[v] = true;
                }
            }
        }

        size_t pos = 0;
        for (idx_t v = 8; v <= MAX_VAL && pos < NEED; ++v) {
            if (dp[v]) {
                first71[pos] = (seq_t)v;
                ++pos;
            }
        }

        free(dp);
        initialized = true;
    }

    if (n <= 71) {
        return first71[n - 1];
    } else {
        /* Linear formula for n ≥ 72. */
        return (seq_t)n + (seq_t)83;
    }
}
