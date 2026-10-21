/*
 * A090632  Given n boxes labeled 1..n, such that box i weighs 3i grams and
 *          can support a total weight of i grams; a(n) = number of stacks of
 *          boxes that can be formed such that no box is squashed.
 * https://oeis.org/A090632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of stacks of boxes 1..n where box i weighs 3i
 *            grams and can support a total weight i grams. The generating
 *            function is G(x) = 1 / ((1-x)^2 * ∏_{i>=0} (1 - x^{3*4^i}))
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The coefficient of x^n in G(x) gives a(n).  We compute the coefficients up to n by
   first expanding 1/(1-x)^2 = Σ_{k>=0} (k+1) x^k.  Then we apply a complete‑knapsack
   DP for each part size 3*4^i (i=0,1,2,…) because 1/(1-x^a) = Σ_{t>=0} x^{a*t}.  Only
   part sizes up to n are relevant; there are at most log_4(n) such sizes.  The DP runs in
   O(n log n) time and O(n) memory. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array of coefficients up to n */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;  /* Allocation failed – return 0 as fallback */

    /* Initial coefficients from 1/(1-x)^2 : dp[k] = k+1 */
    for (idx_t k = 0; k <= n; ++k)
        dp[k] = (seq_t)k + 1;

    /* Apply the product ∏_{i>=0} 1/(1 - x^{3*4^i}) via complete knapsack loops.
       a = 3*4^i. */
    for (idx_t a = 3; a <= n; /* increment in loop */)
    {
        for (idx_t offset = a; offset <= n; ++offset)
            dp[offset] += dp[offset - a];

        /* Prevent overflow of a when multiplied by 4 */
        if (a > n / 4) break;
        a *= 4;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
