/*
 * A029242  Expansion of 1/((1-x^2)*(1-x^9)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A029242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 927, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 2, 9, 10, and 12 */
/* Uses a classic coin‑change DP. */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* Allocate an array large enough to hold 0…n */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp)
        return 0; /* allocation failure */

#ifdef __cplusplus
    /* Construct each element (required for arbitrary‑precision types) */
    for (size_t i = 0; i < len; ++i)
        new (&dp[i]) seq_t();
#endif

    /* Initialise terms: dp[0] = 1, dp[1…n] = 0 */
    dp[0] = 1;
    for (size_t i = 1; i < len; ++i)
        dp[i] = 0;

    /* The available parts */
    static const uint8_t parts[] = { 2, 9, 10, 12 };
    const size_t num_parts = sizeof parts / sizeof parts[0];

    /* Coin‑change DP: for each part, add its contribution */
    for (size_t p = 0; p < num_parts; ++p) {
        uint8_t coin = parts[p];
        for (idx_t i = coin; i <= n; ++i) {
            dp[(size_t)i] += dp[(size_t)(i - coin)];
        }
    }

    seq_t result = dp[(size_t)n];

#ifdef __cplusplus
    /* Destruct elements */
    for (size_t i = 0; i < len; ++i)
        dp[i].~seq_t();
#endif

    free(dp);
    return result;
}
