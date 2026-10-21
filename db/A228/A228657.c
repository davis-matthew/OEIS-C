/*
 * A228657  Number of n X 5 binary arrays with top left value 1 and no two
 *          ones adjacent horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 12*a(n-1) - 27*a(n-2) - 32*a(n-3) + 49*a(n-4) + 20*a(n-5) - 5*a(n-6)
   Initial values: a(1)=5, a(2)=38, a(3)=335, a(4)=2844, a(5)=24109, a(6)=203762.
   This implementation uses dynamic programming over row patterns (no explicit recurrence). */
#include <stdbool.h>

static const int STATE_MASKS[13] = {0,1,2,4,5,8,9,10,16,17,18,20,21};

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Transition matrix between valid row patterns */
    static bool allowed[13][13];
    static bool initialized = false;
    if (!initialized) {
        for (int i = 0; i < 13; ++i) {
            int m = STATE_MASKS[i];
            for (int j = 0; j < 13; ++j) {
                int s = STATE_MASKS[j];
                /* No adjacent ones diagonally/anti‑diagonally between rows */
                allowed[i][j] = ((s >> 1) & m) == 0 && ((s << 1) & m) == 0;
            }
        }
        initialized = true;
    }

    /* DP arrays: number of ways to reach each pattern after current row */
    seq_t dp[13];
    seq_t next[13];
    for (int i = 0; i < 13; ++i) dp[i] = 0;
    for (int i = 0; i < 13; ++i)
        if (STATE_MASKS[i] & 1)           /* top-left cell fixed to 1 */
            dp[i] = 1;

    if (n > 1) {
        for (idx_t row = 2; row <= n; ++row) {
            for (int i = 0; i < 13; ++i) next[i] = 0;
            for (int prev = 0; prev < 13; ++prev) {
                seq_t val = dp[prev];
                for (int curr = 0; curr < 13; ++curr) {
                    if (allowed[prev][curr]) {
                        next[curr] = next[curr] + val;
                    }
                }
            }
            for (int i = 0; i < 13; ++i) dp[i] = next[i];
        }
    }

    seq_t total = 0;
    for (int i = 0; i < 13; ++i) total += dp[i];
    return total;
}
