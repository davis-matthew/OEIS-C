/*
 * A116663  Triangle read by rows: T(n,k) = number of partitions of n into odd
 *          parts and having exactly k parts equal to 1 (n>=0, 1<=k<=n).
 * https://oeis.org/A116663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row containing the nth element of
 *            the triangle read by rows, k is the position in that row. T(r,k)
 *            counts partitions of r into odd parts having exactly k parts
 *            equal to 1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static idx_t _triangle_row(idx_t n)
{
    idx_t low = 0, high = 1;
    /* increase high until it exceeds the bound */
    while ((__uint128_t)high * (high + 1) / 2 <= n) high <<= 1;
    /* binary search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__uint128_t)mid * (mid + 1) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* Determine r (row) and k (column) from the flattened index n */
    idx_t r = _triangle_row(n);
    idx_t offset = n - r * (r + 1) / 2;   /* 0 <= offset <= r */
    idx_t k = offset;                    /* number of ones required */
    idx_t m = r - k;                     /* remaining sum after removing ones */

    /* Dynamic array for partition counts into odd parts >= 3 */
    static seq_t *dp = NULL;
    static size_t dp_cap = 0;
    size_t need = (size_t)m + 1;

    if (need > dp_cap) {
        seq_t *newdp = (seq_t *)malloc(need * sizeof(seq_t));
        if (dp) free(dp);
        dp = newdp;
        dp_cap = need;
    }

    /* Initialize dp[0] = 1, others = 0 */
    dp[0] = 1;
    for (size_t i = 1; i < need; ++i) dp[i] = 0;

    /* Count partitions of i using odd parts >= 3 */
    for (idx_t part = 3; part <= m; part += 2) {
        for (idx_t sum = part; sum <= m; ++sum) {
            dp[sum] = dp[sum] + dp[sum - part];
        }
    }

    return dp[m];
}
