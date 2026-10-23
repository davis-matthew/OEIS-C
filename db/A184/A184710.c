/*
 * A184710  Number of strings of numbers x(i=1..10) in 0..n with Sum_{i=1..10}
 *          i*x(i) = n*10.
 * https://oeis.org/A184710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 25, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Count the number of 10‑tuples (x1,…,x10) with 0 ≤ xi ≤ n
   such that Σ i·xi = 10·n.  This is the coefficient of z^(10*n)
   in Π_{i=1}^{10} (1 + z^i + z^(2i) + … + z^(ni)).
   The routine implements a bounded knapsack DP with a sliding‑window
   optimisation.  The algorithm runs in O(10·n) time and O(10·n) memory.
 */

#ifndef __cplusplus
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    /* Target weighted sum */
    idx_t target = 10 * n;
    size_t size = (size_t)(target + 1);

    /* Allocate DP arrays */
    seq_t *dp;
    seq_t *newdp;

#ifdef __cplusplus
    dp = new seq_t[size];
    newdp = new seq_t[size];
#else
    dp = (seq_t *)malloc(size * sizeof(seq_t));
    newdp = (seq_t *)malloc(size * sizeof(seq_t));
#endif

    if (!dp || !newdp) {
        /* Memory allocation failed – return zero as a safe fallback. */
        return (seq_t)0;
    }

    /* Initialise dp[0]=1, others 0 */
    for (size_t i = 0; i < size; ++i) {
        dp[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1;

    /* Iterate over the 10 weights.  For each weight w we convolve
       the current DP array with (1 + z^w + … + z^(w·n)).  */
    for (idx_t w = 1; w <= 10; ++w) {

        /* Process all residue classes modulo w independently. */
        for (idx_t r = 0; r < w; ++r) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0, s = r; s <= target; ++k, s += w) {
                size_t idx = (size_t)s;
                sum += dp[idx];
                if (k > n) {
                    idx_t idx2 = r + (k - n - 1) * w;
                    sum -= dp[(size_t)idx2];
                }
                newdp[idx] = sum;
            }
        }

        /* Swap pointers for next iteration. */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[(size_t)target];

    /* Free allocated memory. */
#ifdef __cplusplus
    delete[] dp;
    delete[] newdp;
#else
    free(dp);
    free(newdp);
#endif

    return result;
}
