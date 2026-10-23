/*
 * A161102  Number of partitions of n into squares where every part appears at
 *          least 14 times.
 * https://oeis.org/A161102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into squares where every part
 *            appears at least 14 times
 * Range:     u8 n <= 299, u16 n <= 901, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free.h"

seq_t a(idx_t n)
{
    /* For n = 0 the generating function is 1, but the OEIS offset is 1.
       However, handling n = 0 in the DP means that a(0) = 1, which
       does not affect the values for n ≥ 1 that will be queried. */
    if (n == 0)
        return (seq_t)1;

    idx_t N = n;
    seq_t *dp = alloc_seq_array((size_t)(N + 1));
    for (idx_t i = 0; i <= N; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    for (idx_t k = 1; k * k <= N; ++k) {
        idx_t s = k * k;                    /* square value k² */
        idx_t maxm = N / s;                 /* maximum copies of this square */

        if (maxm < 14)
            continue;                      /* can't use this square at all */

        seq_t *newdp = alloc_seq_array((size_t)(N + 1));

        /* zero‑copy case: keep existing ways */
        for (idx_t i = 0; i <= N; ++i)
            newdp[i] = dp[i];

        /* add contributions for each allowed number of copies (>=14) */
        for (idx_t m = 14; m <= maxm; ++m) {
            idx_t val = m * s;              /* total weight contributed */
            for (idx_t i = val; i <= N; ++i) {
                newdp[i] += dp[i - val];
            }
        }

        seq_free(dp);
        dp = newdp;
    }

    seq_t result = dp[N];
    seq_free(dp);
    return result;
}
