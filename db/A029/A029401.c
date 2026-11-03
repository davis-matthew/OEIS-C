/*
 * A029401  Expansion of 1/((1-x^5)*(1-x^7)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A029401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029401: number of partitions of n into parts 5, 7, 10, 12
   Computed by the standard unbounded coin‑change dynamic‑programming
   algorithm. */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static const idx_t parts[] = {5, 7, 10, 12};
    const idx_t part_count = 4;

    /* Allocate vector for dp[0..n] */
    seq_t *dp = mem_new(seq_t, (size_t)(n + 1));
    if (!dp)
        return 0;                      /* allocation failure – safe default */

    dp[0] = 1;                          /* empty partition */

    /* Process each coin; for each coin, all amounts from coin to n are updated */
    for (idx_t p = 0; p < part_count; ++p) {
        idx_t coin = parts[p];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    return result;
}
