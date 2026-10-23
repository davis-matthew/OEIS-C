/*
 * A161148  Number of partitions of n such that each term of the partition is
 *          a squared divisor of n.
 * https://oeis.org/A161148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ∏_{d|n} 1/(1 - x^(d^2))
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Compute ⌊sqrt(n)⌋ for a 64‑bit unsigned integer without a math lib */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > n / mid)  /* mid*mid would overflow or be too large */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* not used – offset is 1, but safety */

    /* Allocation: dp[i] holds the number of partitions of i using the allowed parts */
    seq_t *dp = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;   /* allocation failure – unlikely in test harness */

    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    idx_t sqrt_n = isqrt_u64(n);
    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            idx_t part = d * d;         /* square of the divisor, ≤ n */
            if (part > n) continue;     /* safety – should never happen */
            for (idx_t i = part; i <= n; ++i)
                dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
