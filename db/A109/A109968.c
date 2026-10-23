/*
 * A109968  Number of partitions of n into decimal repdigits of distinct
 *          digits.
 * https://oeis.org/A109968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = # of partitions of n into decimal repdigits of distinct
 *            digits. This DP iterates over the 9 possible digits 1..9. For
 *            each digit we consider all its repdigits (d, dd, ddd, ...) ≤ n
 *            and add the contribution of picking exactly one of them or none
 * Range:     u8 n <= 574, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Allocate two arrays of size n+1 (indices 0..n). */
    seq_t *dp   = alloc_seq_array(n + 1);
    seq_t *next = alloc_seq_array(n + 1);
    if (!dp || !next) {
        /* If allocation fails, free any allocated memory and return 0. */
        if (dp)   free_seq_array(dp);
        if (next) free_seq_array(next);
        return 0;
    }

    /* Initialize dp[0] = 1 and others 0. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* Convert n to seq_t once for comparisons. */
    seq_t n_seq = (seq_t)n;

    /* Process digits 1 through 9. */
    for (int d_int = 1; d_int <= 9; ++d_int) {
        /* Copy current DP state into next (represents skipping this digit). */
        for (idx_t i = 0; i <= n; ++i) next[i] = dp[i];

        /* Generate all repdigits of this digit that are ≤ n. */
        seq_t rep = (seq_t)d_int;      /* first repdigit: d */
        while (rep <= n_seq) {
            idx_t w = (idx_t)rep;      /* weight of this part */

            /* Add this part to all achievable sums. */
            for (idx_t s = w; s <= n; ++s) {
                next[s] += dp[s - w];
            }

            /* Prepare next repdigit: rep*10 + d.  Stop if it would exceed n_seq. */
            if (rep > (n_seq - (seq_t)d_int) / 10)   /* avoid overflow & unnecessary work */
                break;
            rep = rep * 10 + (seq_t)d_int;
        }

        /* Swap dp and next for the next digit. */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    seq_t result = dp[n];

    free_seq_array(dp);
    free_seq_array(next);
    return result;
}
