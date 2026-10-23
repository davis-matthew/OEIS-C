/*
 * A035456  Number of partitions of n into parts 8k+2 or 8k+5.
 * https://oeis.org/A035456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 165, u32 n <= 487, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A035456: Number of partitions of n into parts 8k+2 or 8k+5.
   Formula: a(n) = number of unordered partitions of n whose parts are
   ≡ 2 or 5 (mod 8).  The function computes this by dynamic programming
   and memoises results so that subsequent calls are O(1) for already
   computed n.
*/
#include <stddef.h>                    /* size_t */

#include "../utils/alloc_seq_array.h"  /* alloc_seq_array(size_t) -> seq_t*   */
#include "../utils/free_seq_array.h"   /* free_seq_array(seq_t*)              */

/* static state for memoisation */
static seq_t *dp          = NULL;   /* dp[i] = a(i) for i ≤ max_n */
static idx_t  max_n_computed = 0;   /* largest n for which dp is valid */

/* build dp array for all 0 ≤ i ≤ n */
static void build_dp(idx_t n)
{
    /* free old table if present */
    if (dp != NULL) {
        free_seq_array(dp);
    }

    /* allocate new table (size n+1 because we include dp[0]) */
    dp = alloc_seq_array(n + 1);
    if (dp == NULL) {
        /* allocation failed: return zeroes for all entries (safe) */
        for (idx_t i = 0; i <= n; ++i) {
            dp[i] = (seq_t)0;
        }
        return;
    }

    /* initialise dp[0] = 1 (empty partition) and others to 0 */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        dp[i] = (seq_t)0;
    }

    /* dynamic programming over allowed parts 8k+2 and 8k+5 */
    for (idx_t k = 0; ; ++k) {
        idx_t part1 = 8 * k + 2;
        idx_t part2 = 8 * k + 5;
        if (part1 > n && part2 > n) {
            break;
        }
        if (part1 <= n) {
            for (idx_t i = part1; i <= n; ++i) {
                dp[i] += dp[i - part1];
            }
        }
        if (part2 <= n) {
            for (idx_t i = part2; i <= n; ++i) {
                dp[i] += dp[i - part2];
            }
        }
    }

    max_n_computed = n;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;        /* defined only for n ≥ 1, but safely return 0 */
    }
    if (n > max_n_computed) {
        build_dp(n);
    }
    return dp[n];
}
