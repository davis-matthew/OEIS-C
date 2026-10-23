/*
 * A169217  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A169217: number of reduced words of length n in a Coxeter group with
   12 generators S_i and relations S_i^2 = (S_i S_j)^28 = I.
   Using the rational generating function:
     G(t) = (1 + 2t + 2t^2 + … + 2t^27 + t^28) / (1 - 10t - 10t^2 - … - 10t^27 + 55t^28)
   We obtain the following recurrence:
     a(0) = 1
     a(k) = 2 + 10 * Σ_{i=0}^{k-1} a(i)          for 1 ≤ k ≤ 27
     a(n) = 10 * Σ_{i=1}^{27} a(n-i) - 55 * a(n-28)  for n ≥ 28
*/

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* size of array = n+1, cast n to size_t; n is small (≤ 10000) */
    size_t len = (size_t)n + 1;
    seq_t *arr = (seq_t *)malloc(sizeof(seq_t) * len);
    if (arr == NULL) return (seq_t)0; /* out of memory */

    /* a(0) */
    arr[0] = (seq_t)1;

    /* compute a(1)..a(27) using prefix sums */
    if (n >= 1) {
        seq_t prefix = arr[0];            /* Σ_{i=0}^{k-1} a(i)   */
        for (idx_t k = 1; k <= ((n < 27) ? n : 27); ++k) {
            arr[k] = (seq_t)2 + (seq_t)10 * prefix;
            prefix += arr[k];
        }
    }

    /* if n <= 27 we are done */
    if (n <= 27) {
        seq_t res = arr[n];
        free(arr);
        return res;
    }

    /* compute a(28)..a(n) using the 28‑term recurrence */
    /* initial window sum: Σ_{i=1}^{27} a(i) */
    seq_t wsum = (seq_t)0;
    for (idx_t i = 1; i <= 27; ++i) wsum += arr[i];

    for (idx_t i = 28; i <= n; ++i) {
        arr[i] = (seq_t)10 * wsum - (seq_t)55 * arr[i - 28];
        /* update window: drop arr[i-27] and add arr[i] */
        wsum = wsum + arr[i] - arr[i - 27];
    }

    seq_t res = arr[n];
    free(arr);
    return res;
}
