/*
 * A116372  Number of partitions of n into parts with digital root = 2.
 * https://oeis.org/A116372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of partitions of n into parts with digital root =
 *            2. Parts are numbers congruent to 2 modulo 9 (2, 11, 20, 29, …).
 *            The function uses dynamic programming over the allowed part
 *            sizes, memoises results up to the largest n requested, and
 *            recomputes only when a larger n is needed
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                     /* for size_t */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Static cache of partitions up to maximum n requested. */
    static seq_t *dp   = NULL;    /* dp[i] = number of partitions of i */
    static idx_t dp_sz = 0;       /* current allocated size (dp_sz = max_n + 1) */

    /* Rebuild cache if we are asked for a larger n. */
    if ((idx_t)(n + 1) > dp_sz) {
        /* Free previous cache if it exists. */
        if (dp != NULL)
            free_seq_array(dp);

        /* Allocate new cache big enough for 0 .. n. */
        dp   = alloc_seq_array((size_t)(n + 1));
        dp_sz = (idx_t)(n + 1);

        /* Initialise dp[0] and clear the rest. */
        dp[0] = 1;
        for (idx_t i = 1; i < dp_sz; ++i)
            dp[i] = 0;

        /* Dynamic programming: for each allowed part p, add its contributions. */
        for (idx_t p = 2; p <= n; p += 9) {        /* all numbers ≡ 2 (mod 9) */
            for (idx_t j = p; j <= n; ++j) {
                dp[j] += dp[j - p];
            }
        }
    }

    /* dp[n] is now guaranteed to be correct. */
    return dp[n];
}
