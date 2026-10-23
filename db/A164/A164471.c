/*
 * A164471  Number of binary strings of length n with no substrings equal to
 *          0001 0101 or 1000.
 * https://oeis.org/A164471
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) counts binary strings of length n that avoid the forbidden
 *            4-bit substrings 0001, 0101 and 1000. The computation uses
 *            dynamic programming over the last three bits of the current
 *            prefix. For each state (the last three bits) we keep the number
 *            of valid prefixes that end in that state. Transiti...
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 83, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* for n < 4 the OEIS offset is not defined; return 0 */
    if (n < 4) return 0;

    seq_t dp[8], ndp[8];
    int i;
    /* initialize for length 3: all 8 patterns of three bits are possible */
    for (i = 0; i < 8; ++i) dp[i] = 1;

    for (idx_t len = 3; len < n; ++len) {
        /* clear next state counts */
        for (i = 0; i < 8; ++i) ndp[i] = 0;

        for (i = 0; i < 8; ++i) {
            seq_t cnt = dp[i];
            if (!cnt) continue;

            /* try appending 0 */
            int pat0 = (i << 1);          /* 4‑bit pattern ending with 0 */
            if (!(pat0 == 1 || pat0 == 5 || pat0 == 8)) {
                int newstate0 = pat0 & 7;
                ndp[newstate0] += cnt;
            }

            /* try appending 1 */
            int pat1 = (i << 1) | 1;      /* 4‑bit pattern ending with 1 */
            if (!(pat1 == 1 || pat1 == 5 || pat1 == 8)) {
                int newstate1 = pat1 & 7;
                ndp[newstate1] += cnt;
            }
        }

        /* move to the next length */
        for (i = 0; i < 8; ++i) dp[i] = ndp[i];
    }

    /* sum all valid endings to get the total number of strings */
    seq_t total = 0;
    for (i = 0; i < 8; ++i) total += dp[i];
    return total;
}
