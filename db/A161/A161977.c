/*
 * A161977  Number of reduced words of length n in the Weyl group B_31.
 * https://oeis.org/A161977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Formula: a(n) = coefficient of x^n in  
 *      ∏_{k=1}^{31} (1 - x^{2k}) / (1 - x)  
 *        = ∏_{k=1}^{31} (1 + x + … + x^{2k-1})  
 * The coefficient counts the number of solutions to  
 *      y_1 + y_2 + … + y_31 = n   
 * with 0 ≤ y_k ≤ 2k-1.  
 */
 
#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* allocate two DP tables of size n+1 */
    size_t len = (size_t)(n + 1);
    seq_t *dp    = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *newdp = (seq_t*)malloc(len * sizeof(seq_t));
    if (dp == NULL || newdp == NULL) {
        /* memory allocation failed; return 0 as a safe fallback */
        if (dp) free(dp);
        if (newdp) free(newdp);
        return (seq_t)0;
    }

    /* initialise dp[0] = 1, others 0 */
    for (size_t i = 0; i < len; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* iterate over the 31 factors */
    for (idx_t k = 1; k <= 31; ++k) {
        idx_t limit = 2 * k - 1;          /* maximum exponent in factor k */
        /* clear newdp array */
        for (size_t i = 0; i < len; ++i) newdp[i] = (seq_t)0;

        /* convolution with factor (1 + x + … + x^{limit}) */
        for (idx_t s = 0; s <= n; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;
            /* add val to newdp[s + t] for all allowed t */
            idx_t max_t = limit;
            if (s + max_t > n) max_t = n - s;
            for (idx_t t = 0; t <= max_t; ++t) {
                newdp[s + t] += val;
            }
        }

        /* swap dp and newdp for next iteration */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    /* result is dp[n] */
    seq_t result = dp[n];

    free(dp);
    free(newdp);
    return result;
}
