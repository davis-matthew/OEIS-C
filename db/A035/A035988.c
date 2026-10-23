/*
 * A035988  Number of partitions of n into parts not of the form 21k, 21k+10
 *          or 21k-10. Also number of partitions with at most 9 parts of size
 *          1 and differences between parts at distance 9 are greater than 1.
 * https://oeis.org/A035988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts that are not of the
 *            form 21k, 21k+10, or 21k-10 (equivalently, parts that are not
 *            congruent to 0, 10, 11 modulo 21). The value is computed by
 *            standard DP counting partitions with unlimited copies of each
 *            allowed part size
 * Range:     u8 n <= 16, u16 n <= 45, u32 n <= 138, u64 n <= 460,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate an array of dp values: dp[j] = partitions of j.  */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(len, sizeof(seq_t));  /* calloc is OK in both C and C++ */
    if (!dp) return (seq_t)0;  /* in the unlikely event of allocation failure */

    dp[0] = (seq_t)1;  /* one way to partition 0: use no parts */

    for (idx_t part = 1; part <= n; ++part) {
        idx_t mod = part % (idx_t)21;
        if (mod == (idx_t)0 || mod == (idx_t)10 || mod == (idx_t)11) {
            /* part is disallowed; skip it */
            continue;
        }
        for (idx_t j = part; j <= n; ++j) {
            dp[(size_t)j] += dp[(size_t)(j - part)];  /* unlimited copies of 'part' */
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
