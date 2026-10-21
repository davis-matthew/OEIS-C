/*
 * A073991  Number of strings of length n over Z_6 with trace 3 and subtrace
 *          2.
 * https://oeis.org/A073991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of strings of length n over Z₆ with trace 3 and
 *            sub‑trace 2. Recurrence (for all n ≥ 1, all t,s ∈ {0,…,5}): a(n;
 *            t, s) = Σ_{d=0}^{5} a(n‑1; (t - d) mod 6, (s - d * ((t - d) mod
 *            6)) mod 6) Base: a(1; t, s) = 1 if s == 0 else 0 (no pairs in a
 *            string of length 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for idx_t, seq_t */

seq_t a(idx_t n)
{
    /* dp[t][s] = number of strings of current length with trace t and sub‑trace s (mod 6). */
    seq_t dp[6][6] = {{0}};
    seq_t next[6][6];
    int t, s;

    /* --- initialization for length 1 ------------------------------------- */
    for (t = 0; t < 6; ++t) {
        dp[t][0] = (seq_t)1;          /* sub‑trace is 0 for a single digit */
    }

    /* If n == 1, return the requested value directly. */
    if (n == 1) {
        return dp[3][2];              /* a(1) = 0 */
    }

    /* --- iterate for lengths 2 … n ---------------------------------------- */
    for (idx_t len = 2; len <= n; ++len) {
        /* clear next */
        for (t = 0; t < 6; ++t) {
            for (s = 0; s < 6; ++s) {
                next[t][s] = (seq_t)0;
            }
        }

        /* transition from dp to next */
        for (t = 0; t < 6; ++t) {
            for (s = 0; s < 6; ++s) {
                seq_t cnt = dp[t][s];
                if (!cnt) continue;

                for (int d = 0; d < 6; ++d) {
                    int t_new = t + d;
                    if (t_new >= 6) t_new -= 6;

                    /* new sub‑trace = old + d * old_trace   (mod 6) */
                    int s_new = s + (d * t) % 6;
                    s_new %= 6;

                    next[t_new][s_new] += cnt;
                }
            }
        }

        /* copy next back to dp for the next iteration */
        for (t = 0; t < 6; ++t) {
            for (s = 0; s < 6; ++s) {
                dp[t][s] = next[t][s];
            }
        }
    }

    return dp[3][2];
}
