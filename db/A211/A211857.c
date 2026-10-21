/*
 * A211857  Number of representations of n as a sum of products of distinct
 *          pairs of integers larger than 1, considered to be equivalent when
 *          terms or factors are reordered.
 * https://oeis.org/A211857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 105, u32 n <= 262, u64 n <= 723,
 *            u128 n <= 2162, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A211857: a(n) = number of subsets of the set of unordered pairs
   (i,j) with 2 ≤ j ≤ i and i j ≤ n, whose product values sum to n
   (each pair may be used at most once). */
#include <stdlib.h>
#include <stdbool.h>

/* External linkage */
seq_t a(idx_t n)
{
    /* Maximum n that the function is required to handle. */
    static const idx_t MAX_N = 10000;
    /* Memoised DP array: dp[s] = number of representations of sum s. */
    static seq_t *dp = NULL;
    static bool initialized = false;

    if (!initialized) {
        /* Allocate and initialise the DP array. */
        dp = (seq_t*)calloc((size_t)MAX_N + 1U, sizeof(seq_t));
        if (dp == NULL) return (seq_t)0;          /* safety */
        dp[0] = 1;                                /* empty sum */

        /* Enumerate all distinct unordered pairs (i,j), 2≤j≤i, and
           add their product weight to the 0/1 knapsack DP. */
        for (idx_t j = 2; j * j <= MAX_N; ++j) {
            for (idx_t i = j; i <= MAX_N / j; ++i) {
                idx_t weight = i * j;
                /* Standard 1‑item DP update: iterate sums downward. */
                for (idx_t s = MAX_N; s >= weight; --s) {
                    dp[s] += dp[s - weight];
                }
            }
        }
        initialized = true;
    }

    /* Requests for n larger than MAX_N are undefined in this implementation */
    if (n > MAX_N) return (seq_t)0;
    return dp[n];
}
