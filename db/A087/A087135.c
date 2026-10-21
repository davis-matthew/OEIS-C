/*
 * A087135  Number of positive numbers m such that A073642(m) = n.
 * https://oeis.org/A087135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * p_distinct(n) for n>0, a(0)=1 where p_distinct(n) is
 *            the number of partitions of n into distinct positive integers
 * Range:     u8 n <= 24, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t N = n;
    size_t len = (size_t)(N + 1);
    seq_t *dp = (seq_t*)malloc(len * sizeof(seq_t));
    if (!dp) return (seq_t)0;              /* out‑of‑memory protection */

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= N; ++i) dp[i] = (seq_t)0;

    for (idx_t k = 1; k <= N; ++k) {
        for (idx_t j = N; j >= k; --j) {
            dp[j] += dp[j - k];             /* use operator+ overload if seq_t is a class */
        }
    }

    seq_t res = dp[N];
    res += res;                              /* multiply by 2 */
    free(dp);
    return res;
}
