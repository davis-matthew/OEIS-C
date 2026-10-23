/*
 * A189067  Number of 5Xn binary arrays without the pattern 0 1 0
 *          antidiagonally or horizontally.
 * https://oeis.org/A189067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of 5×n binary arrays without the pattern 0 1 0 *
 *            antidiagonally or horizontally. * * Computation: dynamic
 *            programming on the last two columns. * State = (prev2,prev1)
 *            where each is a 5‑bit column. * For each state we pre‑compute
 *            the list of allowed new * columns that do not create a 010 pa...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* The types seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n) {
    /* Basic cases */
    if (n == 1) return (seq_t)32;    /* 2^5 */
    if (n == 2) return (seq_t)1024;  /* 2^(5*2) */

    /* Pre‑compute allowed transitions once */
    static bool initialized = false;
    static uint8_t next_list[1024][32];
    static uint8_t next_len[1024];

    if (!initialized) {
        for (int s = 0; s < 1024; ++s) {
            int prev2 = s >> 5;
            int prev1 = s & 31;
            uint8_t len = 0;
            for (int curr = 0; curr < 32; ++curr) {
                /* check horizontal 010 in each row */
                bool bad = false;
                for (int r = 0; r < 5 && !bad; ++r) {
                    int b2 = (prev2 >> r) & 1;
                    int b1 = (prev1 >> r) & 1;
                    int b0 = (curr   >> r) & 1;
                    if (b2 == 0 && b1 == 1 && b0 == 0) bad = true;
                }
                /* check antidiagonal 010 for rows 0..2 */
                for (int r = 0; r < 3 && !bad; ++r) {
                    int b2 = (prev2 >> r)     & 1;
                    int b1 = (prev1 >> (r + 1)) & 1;
                    int b0 = (curr   >> (r + 2)) & 1;
                    if (b2 == 0 && b1 == 1 && b0 == 0) bad = true;
                }
                if (!bad) {
                    next_list[s][len++] = (uint8_t)curr;
                }
            }
            next_len[s] = len;
        }
        initialized = true;
    }

    /* DP arrays over the 2‑column state space */
    seq_t dp[1024];
    seq_t newdp[1024];

    /* After 2 columns every pair (prev2,prev1) is feasible once */
    for (int i = 0; i < 1024; ++i) dp[i] = (seq_t)1;

    /* Process columns 3 .. n */
    for (idx_t col = 3; col <= n; ++col) {
        for (int i = 0; i < 1024; ++i) newdp[i] = (seq_t)0;
        for (int s = 0; s < 1024; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            int prev1 = s & 31;
            int base  = prev1 << 5;
            for (uint8_t k = 0; k < next_len[s]; ++k) {
                int ns = base | next_list[s][k];
                newdp[ns] += val;
            }
        }
        for (int i = 0; i < 1024; ++i) dp[i] = newdp[i];
    }

    /* Sum all ending states */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 1024; ++i) total += dp[i];
    return total;
}
