/*
 * A029367  Expansion of 1/((1-x^4)*(1-x^7)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A029367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of partitions of n using parts 4, 7, 10, 12.
 * Computes a(n) for any n ≥ 0 by dynamic programming.
 */
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <cstring>
#endif

seq_t a(idx_t n)
{
    /* static dynamic programming table */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;    /* number of elements allocated */
    static idx_t computed_max = 0; /* largest n that has been computed */

    /* Ensure the array has enough space for indices 0 … n */
    if (n + 1 > dp_size) {
        idx_t new_size = n + 1;
        seq_t *newdp = NULL;

#ifdef __cplusplus
        newdp = new seq_t[new_size];
#else
        newdp = (seq_t *)malloc(new_size * sizeof(seq_t));
#endif
        if (!newdp) return (seq_t)0; /* out of memory */

        /* Copy previously computed values if any */
        if (dp) {
#ifdef __cplusplus
            for (idx_t i = 0; i < dp_size; ++i) newdp[i] = dp[i];
            delete[] dp;
#else
            memcpy(newdp, dp, dp_size * sizeof(seq_t));
            free(dp);
#endif
        }

        dp = newdp;
        dp_size = new_size;
        /* At this point, dp[0] is uninitialized; set the base case */
        dp[0] = (seq_t)1;
    }

    /* Re‑compute the table up to n if we haven't done it already */
    if (n > computed_max) {
        /* Zero out entries to recompute */
        for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;          /* base case */

        static const idx_t parts[4] = {4, 7, 10, 12};
        for (int p_idx = 0; p_idx < 4; ++p_idx) {
            idx_t part = parts[p_idx];
            for (idx_t i = part; i <= n; ++i) {
                dp[i] += dp[i - part];
            }
        }

        computed_max = n;
    }

    return dp[n];
}
