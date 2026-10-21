/*
 * A229227  The partition function G(n,10).
 * https://oeis.org/A229227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..10, k≤n} C(n-1,k-1) * a(n-k) with a(0) = 1.
 *            This is the partition function G(n,10)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* static cache: dp[i] = a(i) for i ≤ computed */
    static seq_t *dp      = NULL;
    static idx_t  cap     = 0;        /* allocated size of dp          */
    static idx_t  computed = 0;       /* maximum index that has been computed */

    /* initial allocation */
    if (cap == 0) {
        cap = 16;                            /* arbitrary small size */
        dp = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!dp) return 0;                    /* unlikely: out of memory   */
        dp[0] = 1;                           /* a(0) = 1 */
        for (idx_t i = 1; i < cap; ++i) dp[i] = 0;
        computed = 0;
    }

    /* If we need values beyond what we have computed, fill them in */
    if (n > computed) {
        /* enlarge array if necessary */
        if (cap <= n) {
            size_t new_cap = (size_t)n + 1;
            seq_t *new_dp  = (seq_t *)realloc(dp, sizeof(seq_t) * new_cap);
            if (!new_dp) return 0;           /* out of memory – unlikely */
            dp = new_dp;
            for (idx_t i = cap; i < new_cap; ++i) dp[i] = 0;
            cap = new_cap;
        }

        /* compute a(computed+1) … a(n) */
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t sum = 0;
            /* block size k can be 1…10 and ≤ i */
            for (idx_t k = 1; k <= 10 && k <= i; ++k) {
                seq_t comb = binom(i - 1, k - 1);   /* C(i-1, k-1) */
                sum += comb * dp[i - k];
            }
            dp[i] = sum;
        }
        computed = n;
    }

    return dp[n];
}
