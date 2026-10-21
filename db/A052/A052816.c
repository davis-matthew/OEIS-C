/*
 * A052816  Expansion of g.f. (1+x)*Product_{m>0} (1 + x^m).
 * https://oeis.org/A052816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000009(n) + A000009(n-1) (number of partitions into
 *            distinct parts, with two sorts of 1's)
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *dp = NULL;          /* DP array: dp[i] = A000009(i) */
static idx_t dp_size = 0;         /* current allocated size of dp (at least n+1) */

/* Compute the array of distinct-partition numbers up to n.
   The array is stored in the static buffer `dp`. */
static void compute_dp(idx_t n)
{
    /* Allocate or reallocate the array if necessary */
    if (dp_size < n + 1) {
        seq_t *tmp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failed */
        dp = tmp;
        dp_size = n + 1;
    }

    /* Initialise the array (sum 0 has one partition, the empty one) */
    for (idx_t i = (idx_t)0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Classic O(n²) DP for partitions into distinct parts */
    for (idx_t part = (idx_t)1; part <= n; ++part) {
        for (idx_t sum = n; sum >= part; --sum) {
            dp[sum] += dp[sum - part];
        }
    }
}

seq_t a(idx_t n)
{
    /* Ensure dp[n] and dp[n-1] are valid */
    compute_dp(n);

    /* Return the required combination */
    if (n == (idx_t)0)
        return dp[0];
    return dp[n] + dp[n - 1];
}
