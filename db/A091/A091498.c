/*
 * A091498  The sixth column of triangle A091492, excluding leading zeros.
 * https://oeis.org/A091498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n,6) + p(n-1,6) – p(n-5,6) where p(m,6) is the number
 *            of partitions of m into parts not exceeding 6 (equivalently,
 *            partitions of m into at most 6 parts)
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Allocate temporary array for partition counts up to n. */
    static seq_t *dp = NULL;
    static size_t dp_cap = 0;
    size_t need = (size_t)n + 1;

    if (need > dp_cap) {
        seq_t *newdp = (seq_t*)malloc(need * sizeof(seq_t));
        if (!newdp) return 0;         /* out‑of‑memory: return 0 */
        if (dp) free(dp);
        dp = newdp;
        dp_cap = need;
    }

    /* Initialize dp[0] and zero the rest. */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* Classic coin‑change DP: parts of size 1…6. */
    for (idx_t part = 1; part <= 6; ++part) {
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    /* Compute a(n) = dp[n] + dp[n-1] – dp[n-5] (with bounds checks). */
    seq_t res = dp[n];
    if (n >= 1)
        res += dp[n - 1];
    if (n >= 5)
        res -= dp[n - 5];
    return res;
}
