/*
 * A156144  Number of partitions of n into parts having in decimal
 *          representation the same digital root as n has.
 * https://oeis.org/A156144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 117, u16 n <= 324, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A156144 – Partitions into parts with the same digital root as n
 *
 *  Implementation:  For each n (n >= 1) compute the residue
 *
 *      r = n % 9
 *
 *  All parts must be congruent to r modulo 9 (with r = 0 representing
 *  digital root 9).  The smallest part in this residue class is
 *
 *      start = (r == 0) ? 9 : r
 *
 *  The number of partitions is then the coefficient of x^n in
 *
 *      \prod_{k >= 0} 1 / (1 - x^{start + 9k})
 *
 *  We compute this by the classic coin–change dynamic programming
 *  with one unlimited supply of each allowed part size.
 *
 *  Complexity:  O(n^2/9) ≈ 1.1·10^7 operations for n = 10 000,
 *  well within the required limits.
 */

#include <stdlib.h>

/* External linkage is required by the build system */
seq_t a(idx_t n)
{
    /* Compute the residue class that all parts must belong to */
    const idx_t r = n % 9;
    const idx_t start = (r == 0) ? 9 : r;

    /* Allocate DP array */
    const size_t N = (size_t)n;
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp) return 0;                      /* Allocation failure, unlikely */

    /* Initialise dp[0] = 1 (empty partition), rest = 0 */
    for (size_t i = 0; i <= N; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Unbounded coin change: for each allowed part size, add to dp */
    for (idx_t part = start; part <= n; part += 9) {
        const size_t p = (size_t)part;
        for (size_t j = p; j <= N; ++j) {
            dp[j] += dp[j - p];
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
