/*
 * A129261  a(n) = coefficient of x^n in Product_{k=0..n} (1 + 2x + 3x^2 +...+
 *          (k+1)*x^k).
 * https://oeis.org/A129261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ∏_{k=0..n} (1 + 2x + 3x^2 + … + (k+1)x^k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps only the coefficients up to degree n.
   For each k it updates the array in O(n) time using a sliding-window
   convolution that avoids explicit nested loops.
   Time complexity: O(n²).  Use 2 arrays of size n+1. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate space for two coefficient arrays */
    size_t sz = (size_t)(n + 1);
    seq_t *arr1 = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *arr2 = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!arr1 || !arr2) {
        free(arr1);
        free(arr2);
        return (seq_t)0;
    }

    seq_t *curr = arr1;   /* current polynomial */
    seq_t *next = arr2;   /* buffer for the next polynomial */

    curr[0] = (seq_t)1;   /* start with product = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t sumS = 0;   /* ∑ old[t]   over current window */
        seq_t sumT = 0;   /* ∑ t*old[t] over current window */

        for (idx_t i = 0; i <= n; ++i) {
            /* add new element to the sliding window */
            sumS += curr[i];
            sumT += (seq_t)i * curr[i];

            /* remove element leaving the window when i > k */
            if (i > k) {
                idx_t idx = i - k - 1;
                sumS -= curr[idx];
                sumT -= (seq_t)idx * curr[idx];
            }

            /* new[i] = (i+1)*sumS - sumT  */
            next[i] = (seq_t)(i + 1) * sumS - sumT;
        }

        /* swap buffers for next iteration */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    seq_t result = curr[n];   /* a(n) is the coefficient x^n */

    free(arr1);
    free(arr2);
    return result;
}
