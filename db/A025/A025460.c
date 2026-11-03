/*
 * A025460  Number of partitions of n into 7 positive cubes.
 * https://oeis.org/A025460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into exactly 7 positive cubes
 * Range:     u8 n <= 32039, u16 n <= 65536, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 124 terms through n = 9006 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* count partitions of n into exactly 7 positive cubes */
seq_t a(idx_t n)
{
    /* minimal possible sum with 7 cubes is 7*1^3 = 7 */
    if (n < 7)
        return 0;

    /* compute floor of cube root of n */
    idx_t max_root = 0;
    for (idx_t r = 1; (r * r * r) <= n; ++r)
        max_root = r;

    /* DP table: dp[k][s] = number of ways to reach sum s using exactly k cubes */
    size_t stride = (size_t)(n + 1);          /* sums 0..n */
    size_t total  = stride * 8;               /* k = 0..7 inclusive */
    seq_t *dp = mem_new(seq_t, total);
    if (!dp)
        return 0; /* allocation failure */

    /* initialize to zero, set base case */
    for (size_t i = 0; i < total; ++i)
        dp[i] = 0;
    dp[0] = 1;  /* dp[0][0] = 1 */

    /* iterate cube values in increasing order – coin change DP */
    for (idx_t cube = 1; cube <= max_root; ++cube) {
        idx_t value = cube * cube * cube;
        for (idx_t sum = value; sum <= n; ++sum) {
            idx_t prev = sum - value;
            /* update counts for k = 1..7 */
            for (int k = 1; k <= 7; ++k) {
                seq_t add = dp[(size_t)(k - 1) * stride + prev];
                dp[(size_t)k * stride + sum] += add;
            }
        }
    }

    seq_t result = dp[(size_t)7 * stride + n];
    mem_free(dp);
    return result;
}
