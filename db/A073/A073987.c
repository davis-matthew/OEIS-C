/*
 * A073987  Number of strings of length n over Z_6 with trace 2 and subtrace
 *          4.
 * https://oeis.org/A073987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of strings of length n over Z_6 with trace = 2 and
 *            subtrace = 4 (mod 6). The computation uses a dynamic programming
 *            over the 6×6 state space (trace, subtrace) with the transition
 *            new_t = (old_t + x) % 6 new_s = (old_s + old_t * x) % 6 where x
 *            ∈ {0,…,5} is the next element of the string
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety – not used in tests */

    /* DP tables: dp[t][s] – number of strings of current length
       with trace t and subtrace s (both modulo 6). */
    seq_t dp[6][6];
    seq_t next[6][6];

    /* Initialisation: length 1 strings.  Each element a has
       trace = a, subtrace = 0. */
    for (int t = 0; t < 6; ++t)
        for (int s = 0; s < 6; ++s)
            dp[t][s] = 0;
    for (int t = 0; t < 6; ++t)
        dp[t][0] = 1;

    if (n == 1) return dp[2][4];

    /* Length‑by‑length transition up to n. */
    for (idx_t len = 2; len <= n; ++len) {
        /* clear next */
        for (int t = 0; t < 6; ++t)
            for (int s = 0; s < 6; ++s)
                next[t][s] = 0;

        for (int t = 0; t < 6; ++t) {
            for (int s = 0; s < 6; ++s) {
                seq_t val = dp[t][s];
                if (val == 0) continue;
                for (int x = 0; x < 6; ++x) {
                    int tnew = t + x;
                    if (tnew >= 6) tnew -= 6;          /* modulo 6 */
                    int snew = s + t * x;
                    snew %= 6;                         /* modulo 6 */
                    next[tnew][snew] += val;
                }
            }
        }

        /* move next to dp */
        for (int t = 0; t < 6; ++t)
            for (int s = 0; s < 6; ++s)
                dp[t][s] = next[t][s];
    }

    return dp[2][4];   /* trace = 2, subtrace = 4 */
}
