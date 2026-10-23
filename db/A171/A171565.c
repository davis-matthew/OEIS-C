/*
 * A171565  Number of partitions of n into odd divisors of n.
 * https://oeis.org/A171565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into odd divisors of n. Computation is
 *            done by a simple DP over the odd divisors of n. The routine
 *            supports arbitrary-precision seq_t and fits in C11 and C++17. It
 *            is memoised for all n up to 10000
 * Range:     u8 n <= 44, u16 n <= 209, u32 n <= 629, u64 n <= 3464,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

#define MAXN 10000   /* maximum n that will be asked for */

static seq_t cache[MAXN + 1];
static unsigned char cached_flag[MAXN + 1] = {0};
static seq_t dp[MAXN + 1];

seq_t a(idx_t n)
{
    if (n > MAXN) {
        /* If a larger n is requested, fall back to a direct
           computation without memoisation.  This branch is
           expected to be unreachable in the tests. */
        idx_t k = n;
        idx_t divs[128];
        size_t dcnt = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                idx_t d1 = d, d2 = k / d;
                if ((d1 & 1) != 0 && dcnt < 128) divs[dcnt++] = d1;
                if (d2 != d1 && (d2 & 1) != 0 && dcnt < 128) divs[dcnt++] = d2;
            }
        }
        seq_t *dp_large = (seq_t *)calloc((size_t)(k + 1), sizeof(seq_t));
        dp_large[0] = 1;
        for (size_t j = 0; j < dcnt; ++j) {
            idx_t d = divs[j];
            for (idx_t s = d; s <= k; ++s)
                dp_large[s] = dp_large[s] + dp_large[s - d];
        }
        seq_t res = dp_large[k];
        free(dp_large);
        return res;
    }

    if (cached_flag[n]) return cache[n];

    /* initialise dp[0..n] to zero (in C: assignment of 0 works for
       both integer and class types). */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;   /* one way to partition 0 */

    /* collect odd divisors of n */
    idx_t divs[128];
    size_t dcnt = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d, d2 = n / d;
            if ((d1 & 1) != 0 && dcnt < 128) divs[dcnt++] = d1;
            if (d2 != d1 && (d2 & 1) != 0 && dcnt < 128) divs[dcnt++] = d2;
        }
    }

    /* classic coin‑change DP where order of parts does not matter */
    for (size_t j = 0; j < dcnt; ++j) {
        idx_t d = divs[j];
        for (idx_t s = d; s <= n; ++s)
            dp[s] = dp[s] + dp[s - d];
    }

    cache[n] = dp[n];
    cached_flag[n] = 1;
    return cache[n];
}
