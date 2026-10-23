/*
 * A131799  Number of partitions of n into parts that are squares or cubes.
 * https://oeis.org/A131799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts that are squares or
 *            cubes
 * Range:     u8 n <= 49, u16 n <= 163, u32 n <= 681, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* allocate DP array of size n+1 */
    size_t N = (size_t)n + 1;
    seq_t *dp = (seq_t*)malloc(N * sizeof(seq_t));
    if (!dp) return 0;                     /* allocation failure guard */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i < N; ++i) dp[i] = (seq_t)0;

    /* build list of distinct parts (squares and cubes up to n) */
    size_t max_parts = (size_t)(sqrt((double)n)) + (size_t)(cbrt((double)n)) + 5;
    uint32_t *parts = (uint32_t*)malloc(max_parts * sizeof(uint32_t));
    if (!parts) { free(dp); return 0; }
    size_t parts_len = 0;

    /* use a temporary boolean array to avoid duplicates (1 space for each value <= n) */
    bool *used = (bool*)calloc((size_t)n + 1, sizeof(bool));
    if (!used) { free(dp); free(parts); return 0; }

    /* squares */
    for (idx_t k = 1; (idx_t)(k * k) <= n; ++k) {
        uint32_t sq = (uint32_t)(k * k);
        if (!used[sq]) {
            used[sq] = true;
            parts[parts_len++] = sq;
        }
    }
    /* cubes */
    for (idx_t k = 1; (idx_t)(k * k * k) <= n; ++k) {
        uint32_t cu = (uint32_t)(k * k * k);
        if (!used[cu]) {
            used[cu] = true;
            parts[parts_len++] = cu;
        }
    }

    /* classic “unbounded knapsack / coin change” DP to count combinations */
    for (size_t idx = 0; idx < parts_len; ++idx) {
        uint32_t p = parts[idx];
        for (size_t i = p; i < N; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[(size_t)n];

    free(dp);
    free(parts);
    free(used);
    return result;
}
