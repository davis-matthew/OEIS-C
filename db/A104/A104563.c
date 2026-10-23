/*
 * A104563  A floretion-generated sequence relating to centered square
 *          numbers.
 * https://oeis.org/A104563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; for n≥1 a(n) = 3*a(n-1) - 4*a(n-2) + 4*a(n-3) - 3*a(n-4)
 *            + a(n-5) + (n==1 || n==4 ? 1 : 0) (derived from the generating
 *            function x*(1+x^3)/((1+x^2)*(1-x)^3)
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* static array for memoisation */
    static seq_t *dp = NULL;           /* sequence cache */
    static idx_t dp_size = 0;          /* next index to initialise (dp_size==size) */
    static idx_t dp_capacity = 0;      /* allocated capacity of dp */

    /* Allocate (and grow) cache if needed */
    if (dp_capacity == 0) {
        dp_capacity = 64;
        dp = (seq_t *)malloc(dp_capacity * sizeof(seq_t));
        dp[0] = 0;                 /* a(0) */
        dp_size = 1;
    }
    if (dp_size <= n) {
        /* Grow to at least n+1 */
        if (dp_capacity <= n) {
            while (dp_capacity <= n) dp_capacity <<= 1;
            dp = (seq_t *)realloc(dp, dp_capacity * sizeof(seq_t));
        }
        for (idx_t i = dp_size; i <= n; ++i) {
            /* Helper values (default 0 for negative indices) */
            seq_t a1 = (i >= 1) ? dp[i - 1] : 0;
            seq_t a2 = (i >= 2) ? dp[i - 2] : 0;
            seq_t a3 = (i >= 3) ? dp[i - 3] : 0;
            seq_t a4 = (i >= 4) ? dp[i - 4] : 0;
            seq_t a5 = (i >= 5) ? dp[i - 5] : 0;

            /* flag is 1 for n==1 or n==4, otherwise 0 */
            seq_t flag = (i == 1 || i == 4) ? (seq_t)1 : (seq_t)0;

            /* Compute recurrence using only unsigned arithmetic;
               intermediate wrap‑around is safe because the final result
               fits in the unsigned type. */
            seq_t val = a1 + a1 + a1;          /* 3*a1   */
            val += a5;                        /* + a5   */
            val += flag;                      /* + flag */
            val -= a2 + a2 + a2 + a2;          /* - 4*a2 */
            val += a3 + a3 + a3 + a3;          /* + 4*a3 */
            val -= a4 + a4 + a4;              /* - 3*a4 */

            dp[i] = val;
        }
        dp_size = n + 1;
    }
    return dp[n];
}
