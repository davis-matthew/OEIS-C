/*
 * A025449  Number of partitions of n into 5 nonnegative cubes.
 * https://oeis.org/A025449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Validated: partial, pass, 108 terms through n = 107 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into 5 nonnegative cubes. */
/* Algorithm: dynamic programming over cubes. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/mem.h"

/* integer cube root: largest k with k³ ≤ x */
static idx_t int_cube_root(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 cube = (__int128)mid * mid * mid;
        if (cube <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    const int MAX_PARTS = 5;
    /* allocate DP array: (n+1) sums × (MAX_PARTS+1) part counts */
    size_t dp_size = (size_t)(n + 1) * (MAX_PARTS + 1);
    seq_t *dp = mem_new(seq_t, dp_size);
    if (!dp) return 0;  /* allocation failed: return 0 */

    /* initialise to zero and set base case */
    for (size_t i = 0; i < dp_size; ++i) dp[i] = 0;
    dp[0] = 1;        /* dp[0*6 + 0] = 1 */

    /* loop over all cube values ≤ n */
    idx_t max_val = int_cube_root(n);   /* floor cube root of n */
    for (idx_t val = 0; val <= max_val; ++val)
    {
        idx_t cube = val * val * val;   /* val^3 ≤ n */
        for (idx_t s = cube; s <= n; ++s)
        {
            size_t base     = s * (MAX_PARTS + 1);
            size_t basePrev = (s - cube) * (MAX_PARTS + 1);
            for (int parts = 1; parts <= MAX_PARTS; ++parts)
            {
                dp[base + parts] += dp[basePrev + parts - 1];
            }
        }
    }

    seq_t result = dp[(size_t)n * (MAX_PARTS + 1) + MAX_PARTS];
    mem_free(dp);
    return result;
}
