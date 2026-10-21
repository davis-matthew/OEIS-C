/*
 * A097876  Number of ways to partition n into sums of squares of nonprimes.
 * https://oeis.org/A097876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Computation: number of partitions of n into sums of squares of nonprime integers. */
seq_t a(idx_t n)
{
    /* Allocate array ways[0..n] */
    seq_t *ways = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!ways) return 0;           /* fallback if allocation fails */

    /* Initialize DP table */
    for (idx_t i = 0; i <= n; ++i)
        ways[i] = 0;
    ways[0] = 1;

    /* Use every square of a nonprime integer k (k = 1..sqrt(n)) as a coin */
    for (idx_t k = 1; k * k <= n; ++k) {
        if (!is_prime(k)) {
            idx_t denom = k * k;
            for (idx_t i = denom; i <= n; ++i)
                ways[i] = ways[i] + ways[i - denom];
        }
    }

    seq_t result = ways[n];
    free(ways);
    return result;
}
