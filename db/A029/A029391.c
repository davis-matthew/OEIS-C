/*
 * A029391  Expansion of 1/((1-x^5)*(1-x^6)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A029391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029391: number of partitions of n into parts 5, 6, 10 and 12 */
/* We use a classic O(n) unbounded‑coin dynamic‑programming algorithm. */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Allocate an array of length n+1, initialized to zero. */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = alloc_seq_array(sz);
    if (!dp)                 /* unlikely, but guard against allocation failure */
        return (seq_t)0;

    /* Base case: one way to make 0 (use no parts). */
    dp[0] = (seq_t)1;

    /* Standard unbounded‑coin DP: iterate over each allowed part. */
    static const idx_t parts[4] = {5, 6, 10, 12};
    for (size_t p = 0; p < 4; ++p) {
        idx_t coin = parts[p];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - coin];
        }
    }

    seq_t result = dp[n];
    seq_free_array(dp);
    return result;
}
