/*
 * A008583  Molien series for Weyl group E_7.
 * https://oeis.org/A008583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = coefficient of x^n in
 *            1/((1-x)*(1-x^3)*(1-x^4)*(1-x^5)*(1-x^6)*(1-x^7)*(1-x^9))
 * Range:     u8 n <= 23, u16 n <= 84, u32 n <= 624, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* the allowed part sizes in the generating function */
    static const unsigned parts[] = {1, 3, 4, 5, 6, 7, 9};
    const size_t num_parts = sizeof(parts) / sizeof(parts[0]);

    /* Allocate an array for the dynamic programming table */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp)
        return 0;   /* out of memory – unlikely for the intended n range */

    dp[0] = 1;                /* base case: one way to obtain sum 0 */
    for (size_t i = 1; i < sz; ++i)
        dp[i] = 0;            /* initialise the rest to zero */

    /* Standard coin‑change DP: for each part size add contributions */
    for (size_t pi = 0; pi < num_parts; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i)
            dp[i] += dp[i - p];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
