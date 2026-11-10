/*
 * A210995  Concatenation of the elements of the n-th row of A079559 assuming
 *          that it is an irregular triangle with row lengths 2^n, n>=0.
 * https://oeis.org/A210995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of A079559(k) for k = 2^n-1 .. 2^(n+1)-2;
 *            A079559(m) = number of partitions of m into distinct parts 2^j-1
 *            (0/1 knapsack DP)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 6 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t lo = ((idx_t)1 << n) - 1;
    idx_t hi = ((idx_t)1 << (n + 1)) - 2;
    uint64_t *dp = (uint64_t *)malloc(((size_t)hi + 1) * sizeof(uint64_t));
    if (!dp) return 0;
    for (idx_t i = 0; i <= hi; ++i) dp[i] = 0;
    dp[0] = 1;
    for (idx_t j = 1; j < 63; ++j) {
        idx_t m = ((idx_t)1 << j) - 1;
        if (m > hi) break;
        for (idx_t r = hi; r >= m; --r) dp[r] += dp[r - m];
    }
    seq_t res = 0;
    for (idx_t k = lo; k <= hi; ++k) {
        res = res * 10 + (seq_t)(dp[k]);
    }
    free(dp);
    return res;
}
