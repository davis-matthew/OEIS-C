/*
 * A029183  Expansion of 1/((1-x^2)*(1-x^4)*(1-x^7)*(1-x^10)).
 * https://oeis.org/A029183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-4) + a(n-7) + a(n-10) - a(n-6) + a(n-9) +
 *            ... (see OEIS A029183)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0)=1, a(1)=0, a(2)=1, a(3)=0, a(4)=2, a(5)=0, a(6)=2, a(7)=1, ... */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1 */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (dp == NULL) {
        /* In a real program you might handle allocation failure; for this
           exercise we simply return 0 on failure. */
        return (seq_t)0;
    }

    /* Initialise all entries to 0 */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Base case: one way to partition 0 (empty sum) */
    dp[0] = (seq_t)1;

    /* Parts that can be used: 2, 4, 7, 10 */
    const idx_t parts[4] = { 2, 4, 7, 10 };
    for (int pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
