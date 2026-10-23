/*
 * A126380  Number of base 26 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A126380:
 *   a(0) = 1
 *   a(1) = 26
 *   For n >= 2, let dp[i][k] be the number of length‑i strings ending
 *   with digit k (0 ≤ k < 26) where adjacent digits differ by at most 1.
 *   Then dp[1][k] = 1 and for i > 1:
 *        dp[i][k] = dp[i-1][k] +
 *                   (k > 0   ? dp[i-1][k-1] : 0) +
 *                   (k+1 < 26 ? dp[i-1][k+1] : 0)
 *   a(n) = Σ_{k=0}^{25} dp[n][k].
 * This counts base‑26 n‑digit numbers (including leading zeros) with
 * adjacent digits differing by one or less. */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    /* dp holds counts for the current length, next for the next length */
    seq_t dp[26], next[26];
    /* length 1 initialization */
    for (idx_t i = 0; i < 26; ++i) dp[i] = (seq_t)1;
    if (n == 1) {
        seq_t sum = 0;
        for (idx_t i = 0; i < 26; ++i) sum += dp[i];
        return sum;
    }
    /* iterate lengths 2 .. n */
    for (idx_t len = 2; len <= n; ++len) {
        for (idx_t k = 0; k < 26; ++k) {
            seq_t val = dp[k];
            if (k > 0) val += dp[k - 1];
            if (k + 1 < 26) val += dp[k + 1];
            next[k] = val;
        }
        /* copy next back to dp for next iteration */
        for (idx_t k = 0; k < 26; ++k) dp[k] = next[k];
    }
    /* sum over final length */
    seq_t total = 0;
    for (idx_t k = 0; k < 26; ++k) total += dp[k];
    return total;
}
