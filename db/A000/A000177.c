/*
 * A000177  Number of partitions of n into 6 squares.
 * https://oeis.org/A000177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..6} p_j(n), where p_j(n) = number of partitions
 *            of n into exactly j positive squares (zeros fill the remaining
 *            parts); computed by a knapsack DP over the squares k^2 <= n
 * Range:     u8 n <= 514, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *a177_dp = 0;   /* a177_dp[j*(a177_N+1)+s] */
static idx_t a177_N = 0;
static int a177_have = 0;

static void a177_build(idx_t N)
{
    size_t W = (size_t)N + 1;
    uint64_t *dp = (uint64_t *)calloc(7 * W, sizeof(uint64_t));
    size_t j, s, k;
    dp[0] = 1;
    for (k = 1; (uint64_t)k * k <= (uint64_t)N; k++) {
        size_t q = k * k;
        for (j = 1; j <= 6; j++) {
            uint64_t *cur = dp + j * W;
            const uint64_t *prev = dp + (j - 1) * W;
            for (s = q; s < W; s++)
                cur[s] += prev[s - q];
        }
    }
    free(a177_dp);
    a177_dp = dp;
    a177_N = N;
    a177_have = 1;
}

seq_t a(idx_t n)
{
    size_t W, j;
    uint64_t total = 0;
    if (!a177_have || n > a177_N) {
        idx_t N = n < 1024 ? 1024 : n;
        if (a177_have && N < 2 * a177_N) N = 2 * a177_N;
        a177_build(N);
    }
    W = (size_t)a177_N + 1;
    for (j = 0; j <= 6; j++)
        total += a177_dp[j * W + (size_t)n];
    return (seq_t)total;
}
