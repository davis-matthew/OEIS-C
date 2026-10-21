/*
 * A073966  Number of strings of length n over Z_5 with trace 1 and subtrace
 *          0.
 * https://oeis.org/A073966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of strings of length n over Z_5 with trace 1 and
 *            subtrace 0. Uses dynamic programming over the 5×5 state space of
 *            (trace, subtrace)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* state index: t*5 + s, where t is trace (0..4) and s is subtrace (0..4) */
    seq_t dp[25] = {0};
    seq_t newdp[25];
    dp[0] = 1;                                 /* dp[trace=0][subtrace=0] = 1 */

    for (idx_t step = 0; step < n; ++step) {
        /* zero newdp */
        for (int j = 0; j < 25; ++j) newdp[j] = 0;

        for (int t = 0; t < 5; ++t) {
            for (int s = 0; s < 5; ++s) {
                seq_t val = dp[t * 5 + s];
                if (!val) continue;
                for (int x = 0; x < 5; ++x) {
                    int tnew = t + x;
                    if (tnew >= 5) tnew -= 5;            /* mod 5 */

                    int snew = s + x * t;
                    snew %= 5;                           /* mod 5 */

                    newdp[tnew * 5 + snew] += val;
                }
            }
        }
        /* copy newdp to dp for next iteration */
        for (int j = 0; j < 25; ++j) dp[j] = newdp[j];
    }
    return dp[1 * 5 + 0];                      /* trace 1, subtrace 0 */
}
