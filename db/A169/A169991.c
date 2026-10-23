/*
 * A169991  Expansion of Product_{i=0..m-1} (1 + x^(2*i+1)) for m=8.
 * https://oeis.org/A169991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of subsets of {1,3,5,7,9,11,13,15} whose sum
 *            equals n (OEIS A169991)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* maximum possible sum = 1+3+5+7+9+11+13+15 = 64 */
    enum { MAX = 64U };
    static seq_t dp[MAX + 1];
    static bool init = false;

    if (!init) {
        for (idx_t s = 0; s <= MAX; ++s)
            dp[s] = 0;
        dp[0] = 1;

        const idx_t coins[8] = {1U, 3U, 5U, 7U, 9U, 11U, 13U, 15U};

        for (idx_t c = 0; c < 8; ++c) {
            idx_t val = coins[c];
            /* iterate backwards to avoid using a coin more than once */
            for (idx_t s = MAX; s >= val; --s)
                dp[s] += dp[s - val];
        }
        init = true;
    }

    return (n <= MAX) ? dp[(idx_t)n] : (seq_t)0;
}
