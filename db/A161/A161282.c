/*
 * A161282  Number of partitions of n into numbers not divisible by 3 where
 *          every part appears at least 4 times.
 * https://oeis.org/A161282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{m≥1, 3∤m} (x^{4m} + x^{5m} +
 *            x^{6m} + …)
 * Range:     u8 n <= 57, u16 n <= 143, u32 n <= 417, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for calloc, free */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* sequence starts at offset 1 */

    size_t sz = (size_t)(n + 1);
    seq_t *dp     = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *newdp  = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp || !newdp) {
        free(dp);   free(newdp);
        return 0;
    }

    dp[0] = 1;   /* empty partition */

    for (idx_t m = 1; m <= n; ++m) {
        if (m % 3 == 0)            /* skip multiples of 3 */
            continue;

        /* start with partitions that do not use part m */
        for (idx_t j = 0; j <= n; ++j)
            newdp[j] = dp[j];

        /* add partitions that use part m at least 4 times */
        idx_t step4 = 4 * m;          /* minimal weight contributed by m */
        for (idx_t j = step4; j <= n; ++j) {
            idx_t max_t = j / m;      /* largest t with t*m <= j */
            for (idx_t t = 4; t <= max_t; ++t)
                newdp[j] += dp[j - t * m];
        }

        /* swap dp and newdp for next m */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[n];
    free(dp);
    free(newdp);
    return result;
}
