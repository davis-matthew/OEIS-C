/*
 * A068711  S(n; 0,1) = S(n; 2,3) where S(n; t,s) is the number of length n
 *          4-ary strings whose digits sum to t mod 4 and whose sum of
 *          products of all pairs of digits sum to s mod 4.
 * https://oeis.org/A068711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = S(n;0,1) where S satisfies S(n;t,s) = S(n-1;t,s) +
 *            S(n-1;t+3,s+3t) + S(n-1;t+2,s+2t) + S(n-1;t+1,s+t) (all
 *            arithmetic modulo 4). The recurrence is derived from appending a
 *            new digit d: t'→(t'+d) mod4, s'→(s'+d·t') mod4. This DP computes
 *            the count of 4‑ary strings of length n that meet the...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* offset is 1; a(0) not defined */
    /* DP tables: dp[t][s] for current length */
    seq_t dp[4][4], next[4][4];
    int t, s, d;

    /* initialise for strings of length 1 */
    for (t = 0; t < 4; ++t)
        for (s = 0; s < 4; ++s)
            dp[t][s] = (s == 0) ? 1 : 0;   /* digit d = t, sum=0 */

    /* iterate from length 2 up to n */
    for (idx_t len = 2; len <= n; ++len) {
        /* zero the next table */
        for (t = 0; t < 4; ++t)
            for (s = 0; s < 4; ++s)
                next[t][s] = 0;

        /* propagate transitions for all possible previous states */
        for (t = 0; t < 4; ++t) {
            for (s = 0; s < 4; ++s) {
                seq_t val = dp[t][s];
                if (val == 0)
                    continue;

                /* append a new digit d = 0,1,2,3 */
                for (d = 0; d < 4; ++d) {
                    int new_t = (t + d) & 3;                /* mod 4 */
                    int new_s = (s + d * t) & 3;            /* mod 4 */
                    next[new_t][new_s] += val;
                }
            }
        }

        /* copy next into dp for the next iteration */
        for (t = 0; t < 4; ++t)
            for (s = 0; s < 4; ++s)
                dp[t][s] = next[t][s];
    }

    return dp[0][1];    /* S(n;0,1) */
}
