/*
 * A164425  Number of binary strings of length n with no substrings equal to
 *          0000 0010 or 1111.
 * https://oeis.org/A164425
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) computed with an automaton-based DP that counts binary
 *            strings of length n avoiding the forbidden substrings 0000,
 *            0010, and 1111. States are defined by the length (0‑3) of the
 *            current suffix and the suffix bits themselves
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 84, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* DP arrays: dp[len][mask], len=0..3, mask<2^len */
    seq_t dp[4][8];
    seq_t ndp[4][8];

    /* initialise dp to zero */
    for (int l = 0; l <= 3; ++l)
        for (int m = 0; m < (1 << l); ++m)
            dp[l][m] = 0;

    dp[0][0] = 1;   /* empty string */

    for (idx_t i = 0; i < n; ++i) {
        /* zero ndp */
        for (int l = 0; l <= 3; ++l)
            for (int m = 0; m < (1 << l); ++m)
                ndp[l][m] = 0;

        /* transition */
        for (int l = 0; l <= 3; ++l) {
            int maxMask = 1 << l;
            for (int m = 0; m < maxMask; ++m) {
                seq_t val = dp[l][m];
                if (val == 0) continue;

                /* bit 0 */
                if (l < 3) {
                    int nl = l + 1;
                    int nm = (m << 1);
                    ndp[nl][nm] += val;
                } else {
                    int w4 = (m << 1);
                    /* check forbidden 4‑bit windows */
                    if (!(w4 == 0 || w4 == 2 || w4 == 15))
                        ndp[3][w4 & 0x7] += val;
                }

                /* bit 1 */
                if (l < 3) {
                    int nl = l + 1;
                    int nm = (m << 1) | 1;
                    ndp[nl][nm] += val;
                } else {
                    int w4 = (m << 1) | 1;
                    if (!(w4 == 0 || w4 == 2 || w4 == 15))
                        ndp[3][w4 & 0x7] += val;
                }
            }
        }

        /* swap dp and ndp (copy ndp into dp) */
        for (int l = 0; l <= 3; ++l) {
            int maxMask = 1 << l;
            for (int m = 0; m < maxMask; ++m)
                dp[l][m] = ndp[l][m];
        }
    }

    /* sum all states for length n */
    seq_t total = 0;
    for (int l = 0; l <= 3; ++l)
        for (int m = 0; m < (1 << l); ++m)
            total += dp[l][m];

    return total;
}
