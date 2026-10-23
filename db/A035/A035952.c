/*
 * A035952  Number of partitions of n into parts not of the form 13k, 13k+4 or
 *          13k-4. Also number of partitions with at most 3 parts of size 1
 *          and differences between parts at distance 5 are greater than 1.
 * https://oeis.org/A035952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not congruent to 0,
 *            4, or 9 modulo 13
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* a(1) is the first term in the OEIS table. For n == 0 we return 0 (unused). */
    if (n == 0) return 0;

    /* Allocate an array dp[0..n] that will hold the number of partitions of each integer. */
    seq_t *dp = NULL;
#ifdef __cplusplus
    /* C++: value‑initialize the array so that every element starts as 0. */
    try { dp = new seq_t[(size_t)(n + 1)](); }
    catch (...) { return 0; }
#else
    /* C: malloc the memory then zero‑initialize it manually. */
    dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
#endif

    dp[0] = 1;                     /* base case: one way to partition 0 */

    /* For each allowed part p, add its contribution to all larger totals. */
    for (idx_t p = 1; p <= n; ++p) {
        idx_t r = p % 13;
        if (r == 0 || r == 4 || r == 9) continue;   /* forbidden residues */
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];

#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
