/*
 * A164466  Number of binary strings of length n with no substrings equal to
 *          0001, 0100, or 1011.
 * https://oeis.org/A164466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n avoiding substrings
 *            0001, 0100, 1011. Computed by a 3‑bit suffix DP
 * Range:     u8 n <= 12, u16 n <= 29, u32 n <= 63, u64 n <= 132,
 *            u128 n <= 270, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* handle small n directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;

    /* Precompute allowed transitions for each 3‑bit state and bit */
    static bool allowed[8][2];
    static int next_state[8][2];
    static bool initialized = false;
    if (!initialized) {
        for (int s = 0; s < 8; ++s) {
            for (int b = 0; b < 2; ++b) {
                int pattern = ((s << 1) | b) & 0xF; /* 4‑bit string */
                bool forb = false;
                if (pattern == 0x1) forb = true;   /* 0001 */
                if (pattern == 0x4) forb = true;   /* 0100 */
                if (pattern == 0xB) forb = true;   /* 1011 */
                allowed[s][b] = !forb;
                next_state[s][b] = (s << 1 | b) & 0x7;
            }
        }
        initialized = true;
    }

    seq_t dp[8];
    seq_t ndp[8];
    /* after 3 bits, each 3‑bit suffix occurs exactly once */
    for (int s = 0; s < 8; ++s) dp[s] = (seq_t)1;
    /* iterate from length 4 up to n */
    for (idx_t len = 4; len <= n; ++len) {
        for (int s = 0; s < 8; ++s) ndp[s] = 0;
        for (int s = 0; s < 8; ++s) {
            for (int b = 0; b < 2; ++b) {
                if (!allowed[s][b]) continue;
                int ns = next_state[s][b];
                ndp[ns] += dp[s];
            }
        }
        /* copy ndp to dp */
        for (int s = 0; s < 8; ++s) dp[s] = ndp[s];
    }

    /* sum over all possible suffixes */
    seq_t total = 0;
    for (int s = 0; s < 8; ++s) total += dp[s];
    return total;
}
