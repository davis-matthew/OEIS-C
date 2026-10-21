/*
 * A083206  a(n) is the number of ways of partitioning the divisors of n into
 *          two disjoint sets with equal sum.
 * https://oeis.org/A083206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if sigma(n) is odd; otherwise a(n) = (number of subsets
 *            of divisors of n summing to sigma(n)/2) / 2
 * Range:     u8 n <= 179, u16 n <= 719, u32 n <= 2519, u64 n <= 9240,
 *            u128 n <= 9240, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>


seq_t a(idx_t n)
{
    /* collect the divisors of n */
    uint64_t divs[200];
    size_t dcnt = 0;
    for (uint64_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[dcnt++] = i;
            if (i != n / i)
                divs[dcnt++] = n / i;
        }
    }

    /* compute sigma(n) = sum of divisors */
    seq_t sigma = 0;
    for (size_t i = 0; i < dcnt; ++i)
        sigma += (seq_t)divs[i];

    if ((sigma & 1) != 0)
        return 0;                /* odd sigma => no partitions */

    /* target sum for each part */
    size_t target = (size_t)(sigma >> 1);
    enum { MAX_DP = 50000 };
    assert(target <= MAX_DP);

    /* dynamic programming over sums; static array avoids constructor issues */
    static seq_t dp[MAX_DP + 1];
    dp[0] = 1;
    for (size_t i = 1; i <= target; ++i)
        dp[i] = 0;

    /* build subset counts */
    for (size_t k = 0; k < dcnt; ++k) {
        size_t d = (size_t)divs[k];
        for (size_t s = target; s >= d; --s)
            dp[s] = dp[s] + dp[s - d];
        if (d == 0)    /* safety guard, though never occurs */
            break;
    }

    /* each partition counted twice (subset and its complement) */
    return dp[target] / 2;
}
