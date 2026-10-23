/*
 * A035977  Number of partitions of n into parts not of the form 19k, 19k+8 or
 *          19k-8. Also number of partitions with at most 7 parts of size 1
 *          and differences between parts at distance 8 are greater than 1.
 * https://oeis.org/A035977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035977 – Number of partitions of n into parts not of
 * the form 19k, 19k+8 or 19k−8 (i.e. parts congruent to 0, 8 or 11 mod 19).
 *
 * The routine implements the classic coin‑change dynamic programming:
 *
 *   dp[0] = 1
 *   for each allowed part p (increasing):
 *       for v = p … n:
 *           dp[v] += dp[v‑p]
 *
 * The table is rebuilt from scratch for each call; the algorithm runs
 * in O(n²) time and requires O(n) additional memory, which is well
 * within the limits for n ≤ 10 000.  The implementation uses only
 * integer arithmetic on the supplied types 'seq_t' and 'idx_t'.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)        /* sequence is undefined for n = 0 */
        return 0;

    /* Allocate DP table: dp[v] holds the number of partitions of v. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return 0;        /* out of memory – return 0 as a best effort */

    dp[0] = 1;                /* empty partition */

    /* Process every allowed part p up to n. */
    for (idx_t p = 1; p <= n; ++p) {
        idx_t r = p % 19;
        if (r == 0 || r == 8 || r == 11)   /* excluded residue classes */
            continue;

        for (idx_t v = p; v <= n; ++v)
            dp[v] += dp[v - p];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
