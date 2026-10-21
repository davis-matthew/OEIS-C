/*
 * A056329  Number of reversible string structures with n beads using exactly
 *          five different colors.
 * https://oeis.org/A056329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = 13 a(n-1) – 48
 *            a(n-2) – 36 a(n-3) + 551 a(n-4) – 683 a(n-5) – 1542 a(n-6) +
 *            3546 a(n-7) + 80 a(n-8) – 4280 a(n-9) + 2400 a(n-10) (derived
 *            from the rational generating function of A056329). Initial
 *            values (n = 1 .. 10): 0,0,0,0,1,9,76,542,3523,21393
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    // Handle trivial cases
    if (n == 0) return 0;   // not used, but safe
    // Initial terms 1..10
    const seq_t init[10] = {
        (seq_t)0,  // a(1)
        (seq_t)0,  // a(2)
        (seq_t)0,  // a(3)
        (seq_t)0,  // a(4)
        (seq_t)1,  // a(5)
        (seq_t)9,  // a(6)
        (seq_t)76, // a(7)
        (seq_t)542,// a(8)
        (seq_t)3523,// a(9)
        (seq_t)21393// a(10)
    };
    if (n <= 10) return init[n-1];

    // Dynamic array for computed values
    seq_t *dp = (seq_t*)malloc((size_t)(n+1) * sizeof(seq_t));
    if (!dp) return 0;        // allocation failure
    dp[0] = (seq_t)0;
    for (idx_t i = 1; i <= 10; ++i) dp[i] = init[i-1];

    for (idx_t i = 11; i <= n; ++i) {
        seq_t res = (seq_t)0;
        /* add positive terms */
        res += dp[i-1] * (seq_t)13;
        res += dp[i-4] * (seq_t)551;
        res += dp[i-7] * (seq_t)3546;
        res += dp[i-8] * (seq_t)80;
        res += dp[i-10] * (seq_t)2400;
        /* subtract negative terms */
        res -= dp[i-2] * (seq_t)48;
        res -= dp[i-3] * (seq_t)36;
        res -= dp[i-5] * (seq_t)683;
        res -= dp[i-6] * (seq_t)1542;
        res -= dp[i-9] * (seq_t)4280;
        dp[i] = res;
    }
    seq_t ans = dp[n];
    free(dp);
    return ans;
}
