/*
 * A089051  Number of ways of writing n as a sum of exactly 6 powers of 2.
 * https://oeis.org/A089051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into exactly 6 powers of 2
 *            (each part is 1,2,4,8,…). Order of parts is irrelevant,
 *            repetitions allowed. Computation uses a simple coin‑change style
 *            DP that counts combinations with an exact number of coins
 * Range:     u8 n <= 104, u16 n <= 9849, u32 n <= 9849, u64 n <= 9849,
 *            u128 n <= 9849, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* quick return for impossible sums */
    if (n < 6)
        return 0;

    /* dp[k][s] : number of ways to reach sum s using exactly k powers of 2 */
    size_t sz = (size_t)(7 * (n + 1));
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) return 0;          /* memory allocation failed */
    /* initialise all entries to 0 */
    for (size_t i = 0; i < sz; ++i) dp[i] = 0;
    dp[0] = 1;                  /* one way to form sum 0 with 0 coins */

    seq_t val = 1;               /* current power of 2 */
    seq_t n_val = (seq_t)n;      /* cast once for comparisons */
    while (val <= n_val) {
        idx_t val_idx = (idx_t)val;          /* start index for this coin */
        for (idx_t s = val_idx; s <= n; ++s) {
            /* for each possible coin count from 1 to 6 */
            for (idx_t k = 1; k <= 6; ++k) {
                size_t cur = (size_t)k * (n + 1) + (size_t)s;
                size_t prev = (size_t)(k - 1) * (n + 1) + (size_t)(s - val_idx);
                dp[cur] += dp[prev];
            }
        }
        val <<= 1;            /* next power of 2 */
    }

    seq_t result = dp[(size_t)6 * (n + 1) + (size_t)n];
    free(dp);
    return result;
}
