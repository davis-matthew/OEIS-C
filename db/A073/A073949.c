/*
 * A073949  Number of strings over Z_3 of length n with trace 0 and subtrace
 *          2.
 * https://oeis.org/A073949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of strings over Z_3 of length n with trace 0 and
 *            subtrace 2
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* DP state: dp[t][s] = number of strings of current length with
       trace t (mod 3) and subtrace s (mod 3).  The recurrence is
       a(n; t, s) = a(n-1; t, s)     // last digit = 0
                    + a(n-1; t+2, s+2t+1)   // last digit = 1
                    + a(n-1; t+1, s+t+1)    // last digit = 2
    The indices are always taken modulo 3. */
    seq_t dp[3][3] = {0};
    dp[0][0] = 1;        /* length 0: empty string has trace 0, subtrace 0 */

    seq_t next[3][3];

    for (idx_t len = 1; len <= n; ++len) {
        for (int t = 0; t < 3; ++t) {
            for (int s = 0; s < 3; ++s) {
                seq_t val = dp[t][s];                                   // last digit 0
                int t1 = (t + 2) % 3;
                int s1 = (s + 2 * t + 1) % 3;                            // last digit 1
                val += dp[t1][s1];
                int t2 = (t + 1) % 3;
                int s2 = (s + t + 1) % 3;                                // last digit 2
                val += dp[t2][s2];
                next[t][s] = val;
            }
        }
        /* copy next to dp for the next iteration */
        for (int t = 0; t < 3; ++t) {
            for (int s = 0; s < 3; ++s) {
                dp[t][s] = next[t][s];
            }
        }
    }
    return dp[0][2];   /* trace 0, subtrace 2 */
}
