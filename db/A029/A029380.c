/*
 * A029380  Expansion of 1/((1-x^5)*(1-x^6)*(1-x^7)*(1-x^10)).
 * https://oeis.org/A029380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 5, 6, 7, 10
 * Range:     u8 n <= 134, u16 n <= 924, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Return the number of partitions of n into parts 5, 6, 7, and 10. */
seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t cap = 0;
    const idx_t parts[4] = {5, 6, 7, 10};
    idx_t i, p;
    seq_t v;

    /* Ensure we have an array big enough for indices up to n. */
    if ((idx_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        if (dp == NULL) {
            dp = seq_alloc(new_cap);
        } else {
            dp = seq_realloc(dp, cap, new_cap);
        }
        cap = new_cap;
    }

    /* Start over: zero out the relevant portion and set the base case. */
    for (i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Classic DP over the four allowed parts. */
    for (idx_t k = 0; k < 4; ++k) {
        p = parts[k];
        if (p > n) continue;
        for (i = p; i <= n; ++i) {
            v = dp[i - p];
            dp[i] = dp[i] + v;
        }
    }

    return dp[n];
}
