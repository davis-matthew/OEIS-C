/*
 * A029070  Expansion of 1/((1-x)*(1-x^4)*(1-x^6)*(1-x^7)).
 * https://oeis.org/A029070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 395, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029070: number of partitions of n into parts 1, 4, 6, and 7.
   Computed by dynamic programming.  For every n ≥ 0 the value a(n) is
   obtained in O(n * 4) time.  The algorithm recomputes the table from
   scratch up to the requested n, which guarantees correct results
   regardless of the order of calls.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;   /* dynamic programming table   */
    static idx_t  size = 0;    /* allocated size of dp array   */

    /* Ensure enough room.  Double the size until n fits.  */
    if (n >= size) {
        idx_t new_size = size ? size << 1 : 1;
        while (new_size <= n) new_size <<= 1;
        dp = (seq_t*)realloc(dp, new_size * sizeof(seq_t));
        if (dp == NULL) return (seq_t)0;   /* unexpected failure   */
        size = new_size;
    }

    /* Recompute the table up to n from scratch.  */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; i++) dp[i] = (seq_t)0;

    /* The allowed parts.  */
    const idx_t parts[4] = {1, 4, 6, 7};

    for (int p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    return dp[n];
}
