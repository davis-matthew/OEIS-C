/*
 * A068620  Number of strings over Z_4 of length n with trace 0 and subtrace
 *          0.
 * https://oeis.org/A068620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of length‑n strings over Z4 with trace 0 and
 *            subtrace 0. Computation uses a DP over all (trace, subtrace)
 *            states modulo 4
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Copyright (C) 2023 The Sequence Project.  */
#ifndef SEQ_A068620_H
#define SEQ_A068620_H
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */
    seq_t dp[4][4];                /* dp[trace][subtrace] for current length */
    seq_t next[4][4];

    /* initialization for length 1: one string for each trace, subtrace always 0 */
    for (int t = 0; t < 4; ++t)
        for (int s = 0; s < 4; ++s)
            dp[t][s] = 0;
    for (int t = 0; t < 4; ++t)
        dp[t][0] = 1;

    if (n == 1) return dp[0][0];   /* a(1) = 1 */

    /* iterate lengths 2 .. n */
    for (idx_t len = 2; len <= n; ++len) {
        /* clear next */
        for (int t = 0; t < 4; ++t)
            for (int s = 0; s < 4; ++s)
                next[t][s] = 0;

        for (int t = 0; t < 4; ++t) {
            for (int s = 0; s < 4; ++s) {
                seq_t val = dp[t][s];
                /* append each possible digit d */
                for (int d = 0; d < 4; ++d) {
                    int t2 = t + d;
                    if (t2 >= 4) t2 -= 4;          /* modulo 4 */
                    int s2 = s + d * t;
                    s2 &= 3;                       /* modulo 4 */
                    next[t2][s2] += val;
                }
            }
        }
        /* move next to dp for next iteration */
        for (int t = 0; t < 4; ++t)
            for (int s = 0; s < 4; ++s)
                dp[t][s] = next[t][s];
    }
    return dp[0][0];
}
#endif /* SEQ_A068620_H */
