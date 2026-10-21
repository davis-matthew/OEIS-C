/*
 * A073950  Number of strings over Z_3 of length n with trace 1 and subtrace
 *          0.
 * https://oeis.org/A073950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of length‑n strings over {0,1,2} whose trace (sum
 *            modulo 3) is 1 and subtrace (sum of pairwise products modulo 3)
 *            is 0. The values are obtained by a simple O(n) dynamic program
 *            on the 3×3 state space (trace, subtrace) modulo 3
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* behaviour below offset does not matter */
        return 0;

    /* dp[trace][subtrace] – counts for the current length */
    seq_t dp[3][3]  = {{0}};
    seq_t next[3][3] = {{0}};

    /* length 1 : one digit 0,1,2 with subtrace 0 */
    for (int t = 0; t < 3; ++t)
        dp[t][0] = 1;

    for (idx_t len = 2; len <= n; ++len) {
        /* reset next array */
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                next[i][j] = 0;

        /* add the next digit d ∈ {0,1,2} */
        for (int t0 = 0; t0 < 3; ++t0) {
            for (int s0 = 0; s0 < 3; ++s0) {
                seq_t cnt = dp[t0][s0];
                if (!cnt) continue;

                /* d = 0 */
                next[t0][s0] += cnt;

                /* d = 1 */
                next[(t0 + 1) % 3][(s0 + t0) % 3] += cnt;

                /* d = 2 */
                next[(t0 + 2) % 3][(s0 + (2 * t0) % 3) % 3] += cnt;
            }
        }

        /* swap dp and next */
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                dp[i][j] = next[i][j];
    }

    return dp[1][0];
}
