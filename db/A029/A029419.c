/*
 * A029419  Expansion of 1/((1-x^6)*(1-x^7)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A029419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 6, 7, 9, and 12
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Directly handle the trivial case to avoid allocating zero‑length arrays */
    if (n == 0)
        return 1;

    /* Allocate an array dp[0..n] that can hold seq_t values */
    seq_t *dp = alloc_seq_array(n + 1);
    if (dp == NULL)
        exit(EXIT_FAILURE);  /* allocation failure */

    /* Initialize all entries to zero; dp[0] is set to 1 below */
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* List of allowed part sizes */
    const idx_t parts[4] = {6, 7, 9, 12};

    /* Standard coin‑change DP: iterate over parts externally to count
       unordered partitions exactly once. */
    for (size_t pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}
