/*
 * A173021  Number of numbers <= n whose binary representation is without
 *          isolated ones or isolated double ones.
 * https://oeis.org/A173021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) counts the integers 0 ≤ m ≤ n whose binary representations
 *            * have no isolated 1's or isolated double 1's; equivalently,
 *            every run of * consecutive 1's has length 0 or at least 3
 * Range:     u8 n <= 7926, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Build bit array of n (MSB first). */
    int bits[65];
    idx_t len = 0;

    if (n == 0) {                /* special case for 0 */
        bits[0] = 0;
        len = 1;
    } else {
        int msb = -1;
        for (int i = 63; i >= 0; --i) {
            if ((n >> i) & 1) { msb = i; break; }
        }
        for (int i = msb; i >= 0; --i) {
            bits[len++] = (n >> i) & 1;
        }
    }

    /* DP arrays: dp[tight][run] */
    seq_t dp[2][4] = {0};
    seq_t ndp[2][4] = {0};
    dp[1][0] = 1;                       /* start: tight=1, no pending run */

    for (idx_t pos = 0; pos < len; ++pos) {
        int bit_val = bits[pos];
        /* clear ndp */
        for (int t = 0; t < 2; ++t)
            for (int r = 0; r < 4; ++r)
                ndp[t][r] = 0;

        for (int tight = 0; tight < 2; ++tight) {
            for (int run = 0; run < 4; ++run) {
                seq_t ways = dp[tight][run];
                if (ways == 0) continue;
                int maxbit = tight ? bit_val : 1;
                for (int bit = 0; bit <= maxbit; ++bit) {
                    int ntight = (tight && (bit == maxbit)) ? 1 : 0;
                    int nrun;
                    if (bit == 0) {
                        if (run == 1 || run == 2) continue; /* invalid ending run */
                        nrun = 0;
                    } else { /* bit == 1 */
                        if (run == 0) nrun = 1;
                        else if (run == 1) nrun = 2;
                        else nrun = 3; /* run 2 or 3 becomes >=3 */
                    }
                    ndp[ntight][nrun] += ways;
                }
            }
        }
        /* copy back */
        for (int t = 0; t < 2; ++t)
            for (int r = 0; r < 4; ++r)
                dp[t][r] = ndp[t][r];
    }

    /* Sum over valid terminal states (run == 0 or run == 3). */
    seq_t result = 0;
    for (int tight = 0; tight < 2; ++tight) {
        result += dp[tight][0];
        result += dp[tight][3];
    }
    return result;
}
