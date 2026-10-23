/*
 * A029158  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^7)*(1-x^10)).
 * https://oeis.org/A029158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 3, 7, and 10
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS A029158.  The function can be called in
 * arbitrary order for any n >= 0.  It memoises results in a static
 * array that grows as needed.
 */
seq_t a(idx_t n)
{
    /* static cache of partition counts up to the largest n seen */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* If we already have a value for this n, return it immediately. */
    if (n < dp_size) {
        return dp[n];
    }

    /* We need to compute up to n.  Allocate/resize the array. */
    idx_t new_size = n + 1;
    dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
    if (!dp) { /* allocation failed – unlikely for small n */
        return 0;
    }

    /* Clear the array.  Note that realloc does not zero the new
     * elements, so we must explicitly set them.
     */
    for (idx_t i = 0; i < new_size; ++i) {
        dp[i] = 0;
    }

    /* Initialize the base case: one way to partition 0. */
    dp[0] = 1;

    /* The allowed parts.  Each part is processed sequentially, which
     * yields the correct partition count.
     */
    const idx_t parts[] = {2, 3, 7, 10};
    size_t np = sizeof(parts) / sizeof(parts[0]);

    for (size_t pi = 0; pi < np; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i < new_size; ++i) {
            dp[i] += dp[i - p];
        }
    }

    dp_size = new_size;
    return dp[n];
}
