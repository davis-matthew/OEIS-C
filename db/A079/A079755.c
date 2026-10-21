/*
 * A079755  Operation count to create all permutations of n distinct elements
 *          using the "streamlined" version of Knuth's Algorithm L
 *          (lexicographic permutation generation).
 * https://oeis.org/A079755
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(3) = 0, a(n) = n * a(n-1) + (n-1) * floor((n-1)/2) for n >= 4
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A079755: Operation count to create all permutations of n distinct positions
 * using Knuth's streamlined Algorithm L.
 * Uses memoization so that any order of calls is handled efficiently.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;              /* below offset an arbitrary value */

    /* static memoization table */
    static seq_t *dp = NULL;          /* dp[i] holds a(i) for i >= 3 */
    static idx_t max_computed = 3;    /* highest n already computed */
    static size_t size = 4;           /* current allocated size (must be > 3) */

    /* ensure room for n */
    if (n + 1 > size) {
        size = n + 1;
        dp = (seq_t *)realloc(dp, size * sizeof(seq_t));
    }

    /* expand table initialization if not done */
    if (dp == NULL) {
        dp = (seq_t *)malloc(size * sizeof(seq_t));
        dp[3] = 0;                    /* a(3) = 0 */
    }

    /* compute missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term1 = (seq_t)i * dp[i - 1];
        seq_t term2 = (seq_t)(i - 1) * (seq_t)((i - 1) / 2);
        dp[i] = term1 + term2;
    }
    max_computed = n;

    return dp[n];
}
