/*
 * A079923  Solution to the Dancing School Problem with n girls and n+4 boys:
 *          f(n,4).
 * https://oeis.org/A079923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permanent of the n × (n+4) (0,1)-matrix B with B(i,j)=1
 *            iff i ≤ j ≤ i+4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 22, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a simple DP over the band of width 5. */
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t dp[32];
    static seq_t ndp[32];
    int i, m, k, m2;

    /* initialise DP for 0 rows processed */
    for (i = 0; i < 32; ++i) dp[i] = 0;
    dp[0] = 1;

    for (idx_t step = 0; step < n; ++step) {
        for (i = 0; i < 32; ++i) ndp[i] = 0;
        for (m = 0; m < 32; ++m) {
            seq_t ways = dp[m];
            for (k = 0; k < 5; ++k) {
                if ((m & (1 << k)) == 0) {          /* column still free */
                    m2 = ((m | (1 << k)) >> 1);     /* shift mask to next row */
                    ndp[m2] = ndp[m2] + ways;      /* accumulate ways */
                }
            }
        }
        for (i = 0; i < 32; ++i) dp[i] = ndp[i];
    }

    seq_t ans = 0;
    for (i = 0; i < 32; ++i) ans = ans + dp[i];
    return ans;
}
