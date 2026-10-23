/*
 * A117546  Number of representations of n as a sum of distinct tribonacci
 *          numbers (A000073).
 * https://oeis.org/A117546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Count of representations of n as a sum of distinct tribonacci
 *            numbers (A117546). The computation uses a 0/1‑knapsack count
 *            over the set of distinct positive tribonacci values
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Build the list of distinct positive tribonacci numbers <= n */
    idx_t trib[32];
    idx_t m = 0;
    if (n >= 1) {
        trib[m++] = 1;          /* first distinct positive tribonacci number */
        idx_t t0 = 0, t1 = 1, t2 = 1;
        for (;;) {
            idx_t next = t0 + t1 + t2;
            if (next > n || next == 0) break;
            if (next != trib[m-1]) trib[m++] = next;
            t0 = t1; t1 = t2; t2 = next;
        }
    }

    /* Dynamic programming array: dp[sum] = number of representations
       of 'sum' using the tribonacci numbers considered so far. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;               /* allocation failure, return 0 */

    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = (seq_t)1;                /* empty sum */

    for (idx_t k = 0; k < m; ++k) {
        idx_t t = trib[k];
        for (idx_t s = n; s >= t; --s) {
            dp[s] += dp[s - t];
            if (s == t) break;     /* avoid underflow of unsigned */
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
