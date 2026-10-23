/*
 * A100677  a(1) = 1; a(n) = sum{1<=k<=n/2, GCD(k,n)=1} a(k).
 * https://oeis.org/A100677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = Σ_{1 <= k <= n/2, gcd(k,n)=1} a(k) (for n >= 2)
 * Range:     u8 n <= 58, u16 n <= 70, u32 n <= 9, u64 n <= 70, u128 n <= 9,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, return 0 for safety */
    static seq_t *dp = NULL;
    static idx_t sz = 0;

    /* Expand dynamic array if necessary */
    if (n >= sz) {
        idx_t new_sz = (n + 1) * 2;          /* growth factor to reduce reallocs */
        seq_t *tmp = (seq_t *)realloc(dp, new_sz * sizeof(seq_t));
        if (!tmp) {                          /* allocation failed */
            /* In production code you might handle this differently */
            return (seq_t)0;
        }
        dp  = tmp;
        sz  = new_sz;
        /* Initialize new entries to 0 */
        for (idx_t i = n + 1; i < sz; ++i) dp[i] = (seq_t)0;
    }

    /* Initialize base case */
    if (dp[1] == (seq_t)0) dp[1] = (seq_t)1;

    /* Compute values up to n if not already computed */
    for (idx_t i = 2; i <= n; ++i) {
        if (dp[i] != (seq_t)0) continue;     /* already computed */
        seq_t sum = (seq_t)0;
        idx_t half = i >> 1;                 /* floor(i/2) */
        for (idx_t k = 1; k <= half; ++k) {
            if (gcd_uint64(k, i) == 1) {
                sum += dp[k];
            }
        }
        dp[i] = sum;
    }

    return dp[n];
}
