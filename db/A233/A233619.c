/*
 * A233619  Number of (1+2)X(n+2) 0..2 arrays with no increasing sequence of
 *          length 3 horizontally or diagonally downwards.
 * https://oeis.org/A233619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP algorithm for A233619.
   a(n) counts 3×(n+2) arrays over digits 0..2 with no strictly
   increasing sequence of length 3 horizontally or along the main
   down‑right diagonal.  The computation uses a state transition DP
   over pairs of consecutive columns. */
#include <stdbool.h>

seq_t a(idx_t n) {
    /* number of columns in the array */
    idx_t L = n + 2;           /* L ≥ 3 for n ≥ 1 */

    /* Pre‑compute allowed transitions between triples of columns:
       allowed[s][t][u] is true iff after columns s and t we may place
       column u without violating any constraint. */
    static bool init = false;
    static bool allowed[27][27][27];
    if (!init) {
        for (int s = 0; s < 27; ++s) {
            int s0 = s / 9;
            int s1 = (s / 3) % 3;
            int s2 = s % 3;
            for (int t = 0; t < 27; ++t) {
                int t0 = t / 9;
                int t1 = (t / 3) % 3;
                int t2 = t % 3;
                for (int u = 0; u < 27; ++u) {
                    int u0 = u / 9;
                    int u1 = (u / 3) % 3;
                    int u2 = u % 3;
                    bool ok = true;
                    if (s0 < t0 && t0 < u0) ok = false;   /* horizontal */
                    if (s1 < t1 && t1 < u1) ok = false;   /* horizontal */
                    if (s2 < t2 && t2 < u2) ok = false;   /* horizontal */
                    if (s0 < t1 && t1 < u2) ok = false;   /* diagonal */
                    allowed[s][t][u] = ok;
                }
            }
        }
        init = true;
    }

    /* DP arrays: dp_cur[s][t] holds the number of arrays
       ending with the last two columns having states s and t. */
    static seq_t dp_cur[27][27];
    static seq_t dp_next[27][27];

    /* Initially, any pair of columns is allowed (no constraints yet). */
    for (int s = 0; s < 27; ++s)
        for (int t = 0; t < 27; ++t)
            dp_cur[s][t] = 1;

    /* For each additional column (from the third up to column L). */
    for (idx_t step = 2; step < L; ++step) {
        /* Clear dp_next */
        for (int s = 0; s < 27; ++s)
            for (int t = 0; t < 27; ++t)
                dp_next[s][t] = 0;

        /* Transition */
        for (int s = 0; s < 27; ++s) {
            for (int t = 0; t < 27; ++t) {
                seq_t val = dp_cur[s][t];
                if (val == 0) continue;
                for (int u = 0; u < 27; ++u) {
                    if (allowed[s][t][u]) {
                        dp_next[t][u] += val;
                    }
                }
            }
        }

        /* Copy dp_next to dp_cur for next iteration */
        for (int s = 0; s < 27; ++s)
            for (int t = 0; t < 27; ++t)
                dp_cur[s][t] = dp_next[s][t];
    }

    /* Sum over all possible last two columns to get the total count. */
    seq_t result = 0;
    for (int s = 0; s < 27; ++s)
        for (int t = 0; t < 27; ++t)
            result += dp_cur[s][t];

    return result;
}
