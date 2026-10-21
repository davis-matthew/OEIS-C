/*
 * A073977  Number of strings of length n over Z_6 with trace 1 and subtrace
 *          0.
 * https://oeis.org/A073977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n;1,0) = number of strings of length n over Z_6 with trace 1
 *            and subtrace 0. Recurrence: A(n,t,s) = Σ_{d=0..5} A(n-1,(t+d)
 *            mod 6, (s+d*t) mod 6) with A(0,0,0) = 1. The function returns
 *            A(n,1,0). The algorithm is O(n) with a tiny constant. It works
 *            for any integer type seq_t, including arbitrary...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 by specification; we ignore n == 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* dp[t][s] holds the number of length‑cur strings with
       trace ≡ t (mod 6) and subtrace ≡ s (mod 6). */
    seq_t dp[6][6] = { {0} };
    dp[0][0] = (seq_t)1;

    seq_t tmp[6][6];

    for (idx_t len = 1; len <= n; ++len) {
        /* reset tmp to zero */
        for (int t = 0; t < 6; ++t) {
            for (int s = 0; s < 6; ++s) {
                tmp[t][s] = (seq_t)0;
            }
        }

        for (int t = 0; t < 6; ++t) {
            for (int s = 0; s < 6; ++s) {
                seq_t cur = dp[t][s];
                if (cur == (seq_t)0) continue;
                for (int d = 0; d < 6; ++d) {
                    int nt = (t + d) % 6;
                    int ns = (s + d * t) % 6;
                    tmp[nt][ns] += cur;
                }
            }
        }

        /* copy tmp into dp for next iteration */
        for (int t = 0; t < 6; ++t) {
            for (int s = 0; s < 6; ++s) {
                dp[t][s] = tmp[t][s];
            }
        }
    }

    return dp[1][0];
}
