/*
 * A145112  Numbers of length n binary words with fewer than 4 0-digits
 *          between any pair of consecutive 1-digits.
 * https://oeis.org/A145112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 67, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A145112: Count of binary words of length n where
   between any two consecutive 1s there are at most 3 zeros.
   Computed using a 6‑state DP automaton. */
seq_t a(idx_t n)
{
    /* dp[0] – no 1 seen yet
       dp[1] – last block ended with 1 and 0 zeros since it
       dp[2] – 1 zero since last 1
       dp[3] – 2 zeros
       dp[4] – 3 zeros
       dp[5] – locked: 4 or more zeros after a 1, no further 1 allowed */
    seq_t dp[6], ndp[6];
    for (int i = 0; i < 6; ++i) dp[i] = 0;
    dp[0] = 1;  /* empty string */

    for (idx_t pos = 0; pos < n; ++pos) {
        for (int i = 0; i < 6; ++i) ndp[i] = 0;

        /* state none */
        ndp[0] += dp[0]; /* put 0 */
        ndp[1] += dp[0]; /* put 1 → z0 */

        /* states z = 0..3 */
        for (int z = 0; z < 4; ++z) {
            seq_t cur = dp[1 + z];
            if (cur == 0) continue;

            /* put 0 */
            if (z < 3)
                ndp[2 + z] += cur;   /* z+1 */
            else /* z == 3 */
                ndp[5] += cur;       /* lock */

            /* put 1 */
            ndp[1] += cur;           /* z0 */
        }

        /* lock state */
        ndp[5] += dp[5]; /* only 0 allowed */

        /* copy back */
        for (int i = 0; i < 6; ++i) dp[i] = ndp[i];
    }

    seq_t ans = dp[0];
    for (int i = 1; i < 6; ++i) ans += dp[i];
    return ans;
}
