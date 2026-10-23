/*
 * A035395  Number of partitions of n into parts 7k or 7k+6.
 * https://oeis.org/A035395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts congruent to 0 or 6
 *            modulo 7. Computed by dynamic programming (unbounded knapsack)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *dp = NULL;       /* dp[i] holds a(i) */
static idx_t dp_size = 0;     /* current array size (dp_size-1 is max n computed) */

static void recompute(idx_t n)
{
    if (dp != NULL) {
        free_seq_array(dp);
    }
    dp = alloc_seq_array(n + 1);   /* indices 0 .. n */
    dp_size = n + 1;

    /* initialise */
    for (idx_t i = 0; i <= n; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;                     /* empty partition */

    /* part 6 (k=0 gives 7k+6 = 6) */
    if (n >= 6) {
        for (idx_t i = 6; i <= n; ++i) {
            dp[i] += dp[i - 6];
        }
    }

    /* iterate over parts of the form 7k (k >= 1) and 7k+6 (k >= 1) */
    for (idx_t k = 1; ; ++k) {
        idx_t p1 = 7 * k;
        if (p1 > n) {
            break;
        }
        for (idx_t i = p1; i <= n; ++i) {
            dp[i] += dp[i - p1];
        }

        idx_t p2 = 7 * k + 6;
        if (p2 <= n) {
            for (idx_t i = p2; i <= n; ++i) {
                dp[i] += dp[i - p2];
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;       /* undefined at offset 0, safe */
    }
    if (dp == NULL || n >= dp_size) {
        recompute(n);
    }
    return dp[n];
}
