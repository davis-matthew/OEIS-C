/*
 * A185229  Number of disconnected 2-regular simple graphs on n vertices with
 *          girth at least 9.
 * https://oeis.org/A185229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n with each part ≥9 and at least
 *            2 parts. This equals P(n) - 1 for n ≥ 9, where P(n) counts
 *            partitions of n with parts ≥9. The implementation computes P(n)
 *            via a classic coin‑change DP
 * Range:     u8 n <= 56, u16 n <= 112, u32 n <= 262, u64 n <= 700,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* For n less than 9 no part can be ≥9, so the answer is 0. */
    if (n < 9) {
        return (seq_t)0;
    }

    /* Allocate one element for each sum 0..n.  Use size_t for malloc. */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) abort();            /* allocation failure */

    dp[0] = (seq_t)1;            /* empty partition */
    for (size_t i = 1; i < sz; ++i) dp[i] = (seq_t)0;

    /* Coin change DP: unlimited parts of size ≥9. */
    for (idx_t part = 9; part <= n; ++part) {
        for (idx_t sum = part; sum <= n; ++sum) {
            dp[sum] += dp[sum - part];
        }
    }

    seq_t result = dp[n] - (seq_t)1;  /* subtract the single‑part partition */
    free(dp);
    return result;
}
