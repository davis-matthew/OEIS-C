/*
 * A035429  Number of partitions of n into parts 7k+1 or 7k+5.
 * https://oeis.org/A035429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of partitions of n into parts congruent to 1 or 5 modulo 7.
   This implements OEIS A035429.  The algorithm is a simple
   dynamic‑programming partition routine that runs in O(n²/7) time and
   O(n) memory. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Allocate array of size n+1 for DP values. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) {
        /* In a real environment we would handle allocation failure.
           For this problem we simply return 0. */
        return (seq_t)0;
    }

    /* dp[0] = 1 represents the empty partition. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        dp[i] = (seq_t)0;
    }

    /* For each allowed part p (p ≡ 1 or 5 mod 7), update dp. */
    for (idx_t p = 1; p <= n; ++p) {
        if (p % 7 == 1 || p % 7 == 5) {
            for (idx_t i = p; i <= n; ++i) {
                dp[i] += dp[i - p];
            }
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
