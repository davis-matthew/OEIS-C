/*
 * A233292  Triangle read by rows: T(n,k) is the number of partitions of n
 *          into at most four parts in which no part exceeds k, 0 <= k <= n.
 * https://oeis.org/A233292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Let r be the largest integer with r·(r+1)/2 ≤ n. Let k =
 *            n – r·(r+1)/2. a(n) = number of partitions of r into at most
 *            four parts each ≤ k
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t row = low;
    idx_t base = row * (row + 1) / 2;
    idx_t col  = n - base;
    if (col > row) col = row;          /* part size cannot exceed the row itself */

    /* DP: dp[j][s] = number of partitions of s into exactly j parts,
       each part ≤ current limit, using parts 1..col.  */
    size_t stride = (size_t)(row + 1);
    seq_t *dp = (seq_t *)calloc(5 * stride, sizeof(seq_t));
    if (!dp)
        return 0;                     /* fallback if allocation fails */

    dp[0] = 1;                         /* dp[0][0] = 1 */

    for (idx_t part = 1; part <= col; ++part) {
        for (idx_t sum = part; sum <= row; ++sum) {
            size_t s = (size_t)sum;
            for (unsigned j = 1; j <= 4; ++j) {
                size_t idx_prev = ((size_t)(j - 1)) * stride + (size_t)(sum - part);
                seq_t add = dp[idx_prev];
                if (add) {
                    size_t idx_curr = ((size_t)j) * stride + s;
                    dp[idx_curr] += add;
                }
            }
        }
    }

    /* Sum over all numbers of parts 0..4 */
    seq_t result = 0;
    for (unsigned j = 0; j <= 4; ++j)
        result += dp[j * stride + (size_t)row];

    free(dp);
    return result;
}
