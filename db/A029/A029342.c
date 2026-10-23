/*
 * A029342  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029342: number of partitions of n into parts 4, 5, 9, and 11 */

#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case directly to avoid allocation */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate a DP array of seq_t elements (size n+1) */
    seq_t *dp = alloc_seq_array((size_t)(n) + 1);
    if (!dp) {
        /* Allocation failed – return 0 as a safe fallback */
        return (seq_t)0;
    }

    /* Initialise array */
    for (idx_t i = 0; i <= n; ++i) {
        dp[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1;                          /* empty partition */

    /* The allowed part sizes */
    const idx_t parts[4] = {4, 5, 9, 11};

    for (int pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}
