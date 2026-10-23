/*
 * A029443  Expansion of 1/((1-x^8)(1-x^9)(1-x^10)(1-x^12)).
 * https://oeis.org/A029443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in 1/((1-x^8)(1-x^9)(1-x^10)(1-x^12))
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = number of partitions of n into the parts 8, 9, 10, and 12. */

#include <stdlib.h>
#include <stddef.h>

static const idx_t coins[4] = {8, 9, 10, 12};

seq_t a(idx_t n)
{
    /* Protect against overflow when converting idx_t to size_t. */
    if (n > (idx_t)(SIZE_MAX - 1)) {
        return 0;
    }

    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) return 0;          /* allocation failure → zero */

    for (size_t i = 0; i < sz; ++i) dp[i] = 0;
    dp[0] = 1;                  /* one way to compose 0 */

    for (size_t ci = 0; ci < 4; ++ci) {
        idx_t coin = coins[ci];
        if (coin > n) continue;
        for (idx_t i = coin; i <= n; ++i) {
            dp[(size_t)i] += dp[(size_t)(i - coin)];
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
