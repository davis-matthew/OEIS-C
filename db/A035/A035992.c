/*
 * A035992  Number of partitions of n into parts not of the form 23k, 23k+4 or
 *          23k-4. Also number of partitions with at most 3 parts of size 1
 *          and differences between parts at distance 10 are greater than 1.
 * https://oeis.org/A035992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not congruent to 0,
 *            4, or 19 modulo 23. For example, a(5) = 6 (1+1+1+1+1, 2+1+1+1,
 *            2+2+1, 3+1+1, 3+2, 5). This implementation uses dynamic
 *            programming and ensures that any call to a(idx_t n) returns the
 *            correct value regardless of the order of calls. It...
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */
#include <stdint.h>  /* uint64_t */

seq_t a(idx_t n)
{
    /* n >= 1 by convention (offset 1). */
    if (n == 0) return 0; /* safeguard for accidental 0 */

    /* static caching: dp[i] = a(i) for i <= dp_size-1 */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* enlarge dp array if needed */
    if (n >= dp_size) {
        /* allocate new size (n+1) */
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!new_dp) {
            /* fatal error, terminate program */
            exit(1);
        }
        dp = new_dp;
        dp_size = new_size;
    }

    /* recompute table from scratch for the whole range [0..n] */
    /* initialize all entries to 0 */
    for (idx_t i = 0; i <= n; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;  /* one way to partition 0 */

    /* dynamic programming over allowed part sizes */
    for (idx_t part = 1; part <= n; ++part) {
        idx_t m = part % 23;
        if (m == 0 || m == 4 || m == 19)
            continue;  /* forbidden part */

        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    return dp[n];
}
