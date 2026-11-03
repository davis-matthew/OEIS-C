/*
 * A088670  Number of partitions of n into distinct decimal repdigit numbers.
 * https://oeis.org/A088670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 2316, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 86 terms through n = 9165 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   A088670 – Number of partitions of n into distinct decimal repdigit numbers.
   Computed by a subset‑sum dynamic program over all repdigits ≤ n.
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Allocate the DP table */
    seq_t *dp = mem_new(seq_t, (size_t)(n + 1));
    if (!dp) return 0;
    for (size_t i = 1; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;                     /* empty sum is a valid partition of 0 */

    /* Determine the number of repdigits needed */
    size_t digits = 0;
    idx_t tmp = n;
    do { ++digits; tmp /= 10; } while (tmp);
    size_t rep_cap = 9 * digits + 1;          /* +1 just to be safe */
    seq_t *rep = mem_new(seq_t, rep_cap);
    if (!rep) { mem_free(dp); return 0; }
    size_t repcnt = 0;

    /* Generate all repdigits ≤ n */
    idx_t repunit = 1;                     /* (10^k – 1)/9 for current k */
    while (1) {
        for (int d = 1; d <= 9; ++d) {
            idx_t val = (idx_t)d * repunit;
            if (val <= n) rep[repcnt++] = (seq_t)val;
        }
        /* Next repunit: repunit = repunit*10 + 1,
           but guard against overflow of idx_t */
        if (repunit > n / 10) break;
        repunit = repunit * 10 + 1;
    }

    /* Subset‑sum counting with distinct parts */
    for (size_t j = 0; j < repcnt; ++j) {
        size_t w = (size_t)rep[j];
        for (size_t s = (size_t)n; s >= w; --s) {
            dp[s] += dp[s - w];
        }
    }

    seq_t result = dp[(size_t)n];
    mem_free(dp);
    mem_free(rep);
    return result;
}
