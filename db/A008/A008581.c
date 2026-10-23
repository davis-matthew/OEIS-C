/*
 * A008581  Molien series for 6-dimensional complex reflection group 4.U_4 (3)
 *          of order 2^9 .3^7 .5.7.
 * https://oeis.org/A008581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in
 *            1/((1-x)(1-x^2)(1-x^3)(1-x^4)(1-x^5)(1-x^7)) Equivalent to the
 *            number of partitions of n with parts in {1,2,3,4,5,7}
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    const idx_t parts[] = {1, 2, 3, 4, 5, 7};
    const size_t m = sizeof(parts) / sizeof(parts[0]);

    if (n == 0)
        return (seq_t)1;

#ifdef __cplusplus
    std::vector<seq_t> dp(n + 1, (seq_t)0);
#else
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        return 0; /* out of memory */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
#endif

    dp[0] = (seq_t)1;

    for (size_t pi = 0; pi < m; ++pi) {
        idx_t part = parts[pi];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];

#ifndef __cplusplus
    free(dp);
#endif
    return result;
}
