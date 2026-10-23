/*
 * A164454  Number of binary strings of length n with no substrings equal to
 *          0001 0011 or 0101.
 * https://oeis.org/A164454
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n that avoid the
 *            substrings 0001, 0011, 0101. The computation is done by a simple
 *            linear‐time DP on a 7‑state automaton that tracks the longest
 *            suffix of the current string that is a prefix of one of the
 *            forbidden patterns
 * Range:     u8 n <= 10, u16 n <= 25, u32 n <= 54, u64 n <= 112,
 *            u128 n <= 228, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

extern seq_t a(idx_t n);

static const int trans[7][2] = {
    /* state 0: ""       */ {1, 0},
    /* state 1: "0"      */ {2, 5},
    /* state 2: "00"     */ {3, 4},
    /* state 3: "000"    */ {3, -1},   /* 0001 is forbidden */
    /* state 4: "001"    */ {6, -1},   /* 0011 is forbidden */
    /* state 5: "01"     */ {6, 0},
    /* state 6: "010"    */ {2, -1}    /* 0101 is forbidden */
};

seq_t a(idx_t n)
{
    if (n == 0) return 1;         /* empty string */
    seq_t dp[7] = {0};
    dp[0] = 1;                    /* start at empty prefix */

    for (idx_t len = 0; len < n; ++len) {
        seq_t ndp[7] = {0};
        for (int s = 0; s < 7; ++s)
            if (dp[s]) {
                for (int b = 0; b < 2; ++b) {
                    int ns = trans[s][b];
                    if (ns >= 0)
                        ndp[ns] += dp[s];
                }
            }
        for (int s = 0; s < 7; ++s)
            dp[s] = ndp[s];
    }

    seq_t total = 0;
    for (int s = 0; s < 7; ++s)
        total += dp[s];
    return total;
}
