/*
 * A011574  Stirling numbers of second kind S2(25,n).
 * https://oeis.org/A011574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Stirling number of the second kind S(25,n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: S(0,0)=1, S(n,0)=0 (n>0), S(n,k)=k*S(n-1,k)+S(n-1,k-1) */

#include <stdbool.h>  /* for bool */

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t cache[26] = {0};   /* cache[0] unused, values for 1..25 */

    if (!initialized) {
        /* dynamic programming for S(25,k) */
        seq_t dp[26] = {0};

        /* S(0,0)=1, all other S(0,k)=0 */
        dp[0] = (seq_t)1;

        for (idx_t i = 1; i <= 25; ++i) {
            /* compute row i using previous row dp[] */
            for (idx_t j = i; j >= 1; --j) {
                dp[j] = (seq_t)j * dp[j] + dp[j - 1];
            }
            /* for i>1, S(i,0)=0 */
            dp[0] = (seq_t)0;
        }
        /* store results; cache[n] holds S(25,n) */
        for (idx_t k = 1; k <= 25; ++k)
            cache[k] = dp[k];
        initialized = true;
    }

    if (n < 1 || n > 25)
        return (seq_t)0;   /* Stirling number is zero outside [1,25] */
    return cache[n];
}
