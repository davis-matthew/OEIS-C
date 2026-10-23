/*
 * A023535  Convolution of natural numbers with A023531.
 * https://oeis.org/A023535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{K-1} k*(k+2) + K*(n - S(K) + 1) where K = max{k
 *            | k(k+3)/2 ≤ n} and S(K) = K(K+3)/2. For n = 1 the result is 0
 * Range:     u8 n <= 50, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t max_k(idx_t n)
{
    /* Find the largest integer k such that k(k+3)/2 <= n. */
    idx_t lo = 0;
    idx_t hi = 1;
    /* Exponentially increase hi until the condition fails. */
    while (((__uint128_t)hi * (hi + 3)) / 2 <= n)
        hi <<= 1;
    /* Binary search on [lo, hi). */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (((__uint128_t)mid * (mid + 3)) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    idx_t K = max_k(n);               /* largest k with k(k+3)/2 ≤ n   */
    seq_t k_minus_1 = (idx_t)(K - 1);
    seq_t k_seq     = (idx_t)K;
    seq_t twoKplus5 = (idx_t)(2 * K + 5);

    /* part1 = (K-1)*K*(2K+5)/6  */
    seq_t part1 = k_minus_1 * k_seq * twoKplus5 / 6;

    /* S(K) = K(K+3)/2  */
    seq_t S_K = ((seq_t)K * (K + 3)) / 2;

    /* part2 = K*(n - S(K) + 1)  */
    seq_t part2 = k_seq * ((seq_t)n - S_K + 1);

    return part1 + part2;
}
