/*
 * A073967  Number of strings of length n over Z_5 with trace 1 and subtrace
 *          1.
 * https://oeis.org/A073967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A073967: Number of length‑n strings over Z₅ with trace 1 and sub‑trace 1.
 *  The recurrence for the joint distribution of trace and sub‑trace is
 *  a(n; t, s) = a(n‑1; t, s)
 *               + a(n‑1; t+4, s+4t+1)
 *               + a(n‑1; t+3, s+3t+4)
 *               + a(n‑1; t+2, s+2t+4)
 *               + a(n‑1; t+1, s+t+1)
 *  where all arithmetic on (t, s) is modulo 5.  We compute a(n)=a(n;1,1)
 *  by dynamic programming over the 25 possible (t, s) pairs.  The
 *  implementation runs in O(n) time and uses a few dozen `seq_t` values,
 *  suitable for arbitrary‑precision builds. */

seq_t a(idx_t n)
{
    /* State arrays: dp[trace][subtrace] */
    seq_t dp[5][5] = {0};
    seq_t newdp[5][5];
    /* Base case: empty string has trace 0 and sub‑trace 0 */
    dp[0][0] = 1;

    for (idx_t len = 1; len <= n; ++len) {
        /* reset newdp to zero */
        for (int t = 0; t < 5; ++t)
            for (int s = 0; s < 5; ++s)
                newdp[t][s] = 0;

        for (int t = 0; t < 5; ++t) {
            for (int s = 0; s < 5; ++s) {
                seq_t cur = dp[t][s];
                if (cur == 0) continue;
                /* append each symbol x ∈ {0,…,4} */
                for (int x = 0; x < 5; ++x) {
                    int nt = (t + x) % 5;
                    int ns = (s + x * t) % 5;   /* sub‑trace update: add x·t */
                    newdp[nt][ns] += cur;
                }
            }
        }
        /* swap dp and newdp for next iteration */
        for (int t = 0; t < 5; ++t)
            for (int s = 0; s < 5; ++s)
                dp[t][s] = newdp[t][s];
    }

    return dp[1][1];   /* trace = 1, sub‑trace = 1 */
}
