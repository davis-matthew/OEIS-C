/*
 * A229229  Number of set partitions of {1,...,n^2} into sets of size at most
 *          n.
 * https://oeis.org/A229229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2)! * [x^(n^2)] exp( Σ_{j=1}^{n} x^j / j! ) * This
 *            implementation uses the simple recurrence: * f[0] = 1, * f[i] =
 *            Σ_{j=1}^{min(n,i)} C(i-1, j-1) * f[i-j] * where f[i] counts
 *            partitions of an i-element set with all block sizes ≤ n. * a(n)
 *            = f[n*n]. * Note that the recurrence is O(n³) in...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, realloc */
#include "../utils/binom.h"   /* binom(idx_t, idx_t) : seq_t */

seq_t a(idx_t n)
{
    /* Handle the trivial cases. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Total number of elements. */
    idx_t N = (idx_t)((seq_t)n * (seq_t)n);
    if (N == 0) return (seq_t)1;

    /* Allocate (or reallocate) the DP array.  The array size will
     * be reused across calls; we keep it static to avoid repeated
     * allocations for small values of n. */
    static seq_t *dp = NULL;
    static idx_t dp_cap = 0;

    if (dp_cap < N + 1) {
        seq_t *newdp = (seq_t *)realloc(dp, (N + 1) * sizeof(seq_t));
        if (!newdp) abort();   /* memory allocation failed */
        dp = newdp;
        dp_cap = N + 1;
    }

    /* Base case. */
    dp[0] = (seq_t)1;

    /* Compute f[i] for i = 1 .. N. */
    for (idx_t i = 1; i <= N; ++i) {
        seq_t sum = (seq_t)0;
        idx_t maxj = (i < n) ? i : n;   /* j runs from 1 to min(n,i) */
        for (idx_t j = 1; j <= maxj; ++j) {
            seq_t comb = binom(i - 1, j - 1);   /* C(i-1, j-1) */
            seq_t prod = comb * dp[i - j];
            sum += prod;
        }
        dp[i] = sum;
    }

    return dp[N];
}
