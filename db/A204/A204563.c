/*
 * A204563  Permanent of the n-th principal submatrix of A204562.
 * https://oeis.org/A204563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permanent of the n X n matrix M(i,j) = floor((i+j+3)/2) -
 *            floor((i+j+3)/4), 1<=i,j<=n (A204562 as a symmetric matrix).
 *            Computed by a subset DP over columns used, row by row
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static int A204563_entry(int i, int j)
{
    int s = i + j + 3;
    return s / 2 - s / 4;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    int m = (int)n;
    size_t full = (size_t)1 << m;
    seq_t *dp = mem_new(seq_t, full);
    dp[0] = 1;
    for (size_t mask = 1; mask < full; mask++) {
        int row = 0;
        for (size_t t = mask; t; t >>= 1)
            row += (int)(t & 1);   /* popcount = row index (1-based) */
        seq_t sum = 0;
        for (int j = 0; j < m; j++) {
            if (mask & ((size_t)1 << j))
                sum += dp[mask ^ ((size_t)1 << j)] * A204563_entry(row, j + 1);
        }
        dp[mask] = sum;
    }
    seq_t result = dp[full - 1];
    mem_free(dp);
    return result;
}
