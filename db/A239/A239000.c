/*
 * A239000  Number of partitions of n using Fibonacci numbers > 2.
 * https://oeis.org/A239000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in 1/∏_{i≥4} (1−x^{F(i)})
 * Range:     u8 n <= 86, u16 n <= 297, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here F(i) is the i‑th Fibonacci number; we use only the ones > 2. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;        /* dynamic array holding partition counts */
    static size_t dp_cap = 0;       /* current capacity (dp_cap == max_n + 1) */

    /* If a larger n is requested, recompute the dp array up to that n. */
    if (n >= dp_cap) {
        /* free the old table if it exists */
        if (dp)
            free(dp);

        /* allocate a fresh array of size n+1, zero‑initialized. */
        size_t size = (size_t)n + 1;
        dp = (seq_t *)calloc(size, sizeof(seq_t));
        if (!dp)          /* allocation failure: return 0 as a safe fallback */
            return 0;

        dp[0] = 1;               /* empty partition */

        /* Generate Fibonacci numbers > 2 up to n and perform the classic
           coin‑change DP for each as a “coin”. */
        idx_t f1 = 1, f2 = 1;
        while (1) {
            idx_t f3 = f1 + f2;   /* next Fibonacci number   */
            f1 = f2;
            f2 = f3;
            if (f3 > n)           /* stop once the number exceeds n */
                break;
            if (f3 > 2) {         /* only Fibonacci numbers > 2 are allowed */
                idx_t coin = f3;
                for (idx_t i = coin; i <= n; ++i)
                    dp[i] += dp[i - coin];
            }
        }

        dp_cap = size;           /* remember the new capacity */
    }

    /* The array now contains a[0] … a[n]. Return the requested value. */
    return dp[n];
}
