/*
 * A073973  Number of strings of length n over Z_6 with trace 0 and subtrace
 *          2.
 * https://oeis.org/A073973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of length‑n strings over Z₆ with trace 0 and
 *            subtrace 2. Recurrence: for a state (t,s) of length k and a new
 *            digit d t' = (t + d) mod 6 s' = (s + t*d) mod 6 a(k+1; t', s')
 *            += a(k; t, s). A dynamic programming table of size 6×6 gives all
 *            values
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset is 1; nothing defined below 1 */

    /* two 2‑D arrays for current and next length */
    seq_t cur[6][6];
    seq_t nxt[6][6];

    /* initialise for length 1: trace = digit, subtrace = 0 */
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            cur[i][j] = 0;
    for (int d = 0; d < 6; ++d)
        cur[d][0] = 1;                /* one string consisting of digit d */

    /* iterate to required length */
    for (idx_t len = 2; len <= n; ++len) {
        /* clear next array */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                nxt[i][j] = 0;

        /* transition from each state of length len‑1 */
        for (int t = 0; t < 6; ++t) {
            for (int s = 0; s < 6; ++s) {
                seq_t val = cur[t][s];
                if (val == 0) continue;
                for (int d = 0; d < 6; ++d) {
                    int t2   = (t + d) % 6;
                    int prod = (t * d) % 6;
                    int s2   = (s + prod) % 6;
                    nxt[t2][s2] += val;
                }
            }
        }

        /* copy nxt into cur for next iteration */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* answer: trace 0, subtrace 2 */
    return cur[0][2];
}
