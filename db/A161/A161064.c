/*
 * A161064  Number of partitions of n into powers of two minus one where every
 *          part appears at least 2 times.
 * https://oeis.org/A161064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Generating function a(n) = coefficient of x^n in ∏_{j≥1} (1 +
 *            x^(2*(2^j-1))/(1 - x^(2^j-1)))
 * Range:     u8 n <= 83, u16 n <= 352, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts partitions of n into parts 2^k-1, each part appearing at least twice.   */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    size_t N = (size_t)n;
    /* dp[s] – number of valid partitions of s using parts processed so far   */
    seq_t *dp = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0;
    dp[0] = (seq_t)1;

    /* iterate over part sizes d = 2^k - 1  (k >= 1) */
    for (idx_t pow = 2; pow - 1 <= n; pow <<= 1) {
        idx_t d = pow - 1;

        /* prefix[s] = dp[s] + prefix[s-d]  (prefix of length dp[s], dp[s-d], dp[s-2d], ...)  */
        seq_t *prefix = (seq_t*)calloc(N + 1, sizeof(seq_t));
        for (size_t s = 0; s <= N; ++s) {
            seq_t val = dp[s];
            if (s >= (size_t)d) val += prefix[s - d];
            prefix[s] = val;
        }

        /* newdp[s] = prefix[s] - (s>=d ? dp[s-d] : 0)   */
        seq_t *newdp = (seq_t*)calloc(N + 1, sizeof(seq_t));
        for (size_t s = 0; s <= N; ++s) {
            seq_t val = prefix[s];
            if (s >= (size_t)d) val -= dp[s - d];
            newdp[s] = val;
        }

        free(dp);
        free(prefix);
        dp = newdp;
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
