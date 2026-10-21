/*
 * A098965  Number of integer partitions of n into distinct parts > 1 with a
 *          part dividing all the other parts.
 * https://oeis.org/A098965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n, d<n} A025147(d-1) (number of partitions of n
 *            into distinct parts >1 with a part dividing all others)
 * Range:     u8 n <= 65, u16 n <= 173, u32 n <= 501, u64 n <= 1629,
 *            u128 n <= 5727, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;
static idx_t dp_size = 0;

static void ensure_dp(idx_t upTo)
{
    if (upTo <= dp_size) return;
    /* Allocate new array */
    seq_t *newdp = (seq_t*)malloc((upTo + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= upTo; ++i) newdp[i] = (seq_t)0;
    newdp[0] = (seq_t)1;
    /* Compute partitions into distinct parts >= 2 */
    for (idx_t k = 2; k <= upTo; ++k) {
        for (idx_t s = upTo; s >= k; --s) {
            newdp[s] += newdp[s - k];
        }
    }
    /* Free previous array if any */
    if (dp != NULL) free(dp);
    dp = newdp;
    dp_size = upTo;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* offset is 1 */
    /* Prepare dp up to n-1 */
    ensure_dp(n - 1);

    seq_t result = (seq_t)0;
    /* Sum over divisors m of n that are strictly less than n */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;
        idx_t m1 = i;
        idx_t m2 = n / i;

        if (m1 < n) {
            result += dp[m1 - 1];
        }
        if (m2 != m1 && m2 < n) {
            result += dp[m2 - 1];
        }
    }
    return result;
}
