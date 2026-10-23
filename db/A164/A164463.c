/*
 * A164463  Number of binary strings of length n with no substrings equal to
 *          0001 0100 or 0110.
 * https://oeis.org/A164463
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For the sequence A164463:
 *   a(n) = number of binary strings of length n with no substrings
 *   0001, 0100, or 0110.  The DP uses a 3-bit suffix automaton
 *   and forbids the 4‑bit patterns 1 (0001), 4 (0100) or 6 (0110). */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t dp[8] = {0};
    seq_t nxt[8] = {0};

    dp[0] = 1;                       // empty string
    for (idx_t len = 0; len < n; ++len) {
        for (int s = 0; s < 8; ++s) nxt[s] = 0;

        for (int mask = 0; mask < 8; ++mask) {
            seq_t cur = dp[mask];
            if (!cur) continue;
            for (int bit = 0; bit <= 1; ++bit) {
                int ns = ((mask << 1) | bit) & 7;
                if (len + 1 >= 4) {   // a 4‑bit pattern is formed now
                    int pat = (((mask << 1) | bit) & 15);
                    if (pat == 1 || pat == 4 || pat == 6) continue; // forbidden
                }
                nxt[ns] += cur;
            }
        }
        for (int s = 0; s < 8; ++s) dp[s] = nxt[s];
    }

    seq_t total = 0;
    for (int s = 0; s < 8; ++s) total += dp[s];
    return total;
}
