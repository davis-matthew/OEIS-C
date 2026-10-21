/*
 * A056138  Number of ways in which n can be the shorter leg (shortest side)
 *          of an integer-sided right triangle.
 * https://oeis.org/A056138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer m > n such that n² + m² is a perfect
 *            square. Computed by enumerating divisor pairs (u,v) of n² with u
 *            < v, same parity, and checking if (v–u)/2 > n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   dfs_div, factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_uint64.h"
#include "../utils/dfs_div.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t N  = (uint64_t)n;
    uint64_t N2 = N * N;

    uint64_t fac[20];
    int exp[20];
    int cnt;
    factor_uint64(N, fac, exp, &cnt);

    uint64_t ans = 0;
    dfs_div(1, 0, cnt, N, N2, fac, exp, &ans);
    return (seq_t)ans;
}
