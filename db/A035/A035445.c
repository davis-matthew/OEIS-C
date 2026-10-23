/*
 * A035445  Number of partitions of n into parts 8k or 8k+5.
 * https://oeis.org/A035445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 8k or 8k+5
 *            Computation: dynamic programming over the parts. For each
 *            allowed part c we add dp[i] += dp[i-c]. The function memoises
 *            the dp array and recomputes it from scratch whenever a larger n
 *            is requested
 * Range:     u8 n <= 89, u16 n <= 211, u32 n <= 581, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;          /* dp[i] = a(i) for i <= max_computed */
static size_t dp_size = 0;        /* current allocated size in elements */
static idx_t max_computed = 0;    /* largest n for which dp is correct */

seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) is not defined; return 0 */
        return 0;

    /* Allocate or grow the dp array to hold indices 0..n */
    if ((size_t)n + 1 > dp_size) {
        size_t new_size = (dp_size > 0) ? dp_size : 1;
        while (new_size <= (size_t)n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        dp = tmp;
        /* initialise the newly allocated entries to 0 */
        for (size_t i = dp_size; i < new_size; ++i) dp[i] = 0;
        dp_size = new_size;
    }

    /* If a larger n is requested, recompute the DP from scratch up to n */
    if (n > max_computed) {
        max_computed = n;
        dp[0] = 1;           /* one way to partition 0: use no parts */
        /* Set dp[1..n] to 0 before processing parts */
        for (size_t i = 1; i <= (size_t)n; ++i) dp[i] = 0;

        /* Process all allowed parts up to n */
        for (idx_t c = 5; c <= n; ++c) {
            if (c % 8 == 0 || c % 8 == 5) { /* part is allowed */
                for (idx_t i = c; i <= n; ++i) {
                    dp[(size_t)i] += dp[(size_t)(i - c)];
                }
            }
        }
    }

    return dp[(size_t)n];
}
