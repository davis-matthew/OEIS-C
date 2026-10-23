/*
 * A169307  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^30 = I.
 * https://oeis.org/A169307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced words of length n in the Coxeter group
 *            with six generators S_i (i = 1..6), S_i^2 = I and (S_i S_j)^30 =
 *            I for i ≠ j. The computation is performed by dynamic programming
 *            that forbids a run of 30 alternating letters, which is the only
 *            forbidden subword for reduced words
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset n = 0, 1 directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* dp1[l]   – number of words of current length with run length 1
                  (only possible at length 1).
       dp2[l][p][r] – number of words ending with letter l,
                      previous letter p (p ≠ l), and
                      current alternating run length r (2 ≤ r ≤ 29)   */
    seq_t dp1[6] = {0};
    seq_t dp2[6][6][31] = {{0}};           /* indices: last, prev, len   */
    seq_t newdp2[6][6][31];

    /* Initialise words of length 1 */
    for (int i = 0; i < 6; ++i) dp1[i] = (seq_t)1;

    /* Iterate lengths from 2 up to n */
    for (idx_t len = 2; len <= n; ++len)
    {
        /* zero newdp2 */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                for (int k = 0; k <= 30; ++k)
                    newdp2[i][j][k] = (seq_t)0;

        /* Transitions from dp1 → dp2: only at length 2 */
        if (len == 2)
        {
            for (int last = 0; last < 6; ++last)
            {
                seq_t cur = dp1[last];
                if (cur == (seq_t)0) continue;
                for (int nxt = 0; nxt < 6; ++nxt)
                {
                    if (nxt == last) continue;
                    newdp2[nxt][last][2] += cur;
                }
            }
        }

        /* Transitions from dp2 → dp2 for all lengths */
        for (int last = 0; last < 6; ++last)
        {
            for (int prev = 0; prev < 6; ++prev)
            {
                if (last == prev) continue;
                for (int r = 2; r <= 29; ++r)
                {
                    seq_t cur = dp2[last][prev][r];
                    if (cur == (seq_t)0) continue;
                    for (int nxt = 0; nxt < 6; ++nxt)
                    {
                        if (nxt == last) continue;
                        if (nxt == prev)
                        {
                            /* Continue alternating run if it does not reach 30 */
                            if (r + 1 <= 29)
                                newdp2[nxt][last][r + 1] += cur;
                        }
                        else
                        {
                            /* Break the run; start a new alternating pair of length 2 */
                            newdp2[nxt][last][2] += cur;
                        }
                    }
                }
            }
        }

        /* Move newdp2 → dp2 */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                for (int k = 0; k <= 30; ++k)
                    dp2[i][j][k] = newdp2[i][j][k];

        /* dp1 is no longer needed beyond length 1 */
        for (int i = 0; i < 6; ++i) dp1[i] = (seq_t)0;
    }

    /* Sum all valid states for the final length */
    seq_t total = (seq_t)0;
    for (int last = 0; last < 6; ++last)
    {
        for (int prev = 0; prev < 6; ++prev)
        {
            if (last == prev) continue;
            for (int r = 2; r <= 29; ++r)
                total += dp2[last][prev][r];
        }
    }
    return total;
}
