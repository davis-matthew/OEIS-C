/*
 * A169536  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: 
 *   a(0) = 1
 *   for 1 ≤ n ≤ 33:  a(n) = 41 * Σ_{i=1}^{n} a(n-i) + 2
 *   a(34)   = 41 * Σ_{i=1}^{33} a(i) + 1
 *   for n ≥ 35:    a(n) = 41 * Σ_{i=1}^{33} a(n-i) – 861 * a(n-34)
 * The recurrence follows from the generating function
 *   (t^34 + 2t^33 + … + 2t + 1) / (861t^34 – 41t^33 – … – 41t + 1).
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for all terms up to n */
    size_t N = (size_t)n + 1;
    seq_t *arr = (seq_t *)malloc(N * sizeof(seq_t));
    if (arr == NULL) {      /* allocation failed – unlikely for small n   */
        return (seq_t)0;
    }

    const seq_t K41  = (seq_t)41;
    const seq_t K861 = (seq_t)861;
    const seq_t TWO  = (seq_t)2;
    const seq_t ONE  = (seq_t)1;

    /* a(0) */
    arr[0] = ONE;
    seq_t cumSum = arr[0];   /* sum of a[0]..a[i] that have been computed */

    /* 1 ≤ n ≤ 33 */
    for (idx_t i = 1; i <= n && i <= 33; ++i) {
        arr[i] = K41 * cumSum + TWO;
        cumSum += arr[i];
    }

    /* n may be 34 or less */
    if (n >= 34) {
        /* a(34) = 41*Σ_{i=1}^{33}a(i) + 1 */
        seq_t sum33 = cumSum - arr[0];   /* Σ a[1]..a[33] */
        arr[34] = K41 * sum33 + ONE;
        cumSum += arr[34];

        /* For n = 34, we're done */
        if (n == 34) {
            seq_t result = arr[34];
            free(arr);
            return result;
        }

        /* n ≥ 35: recurrence with sliding window of last 33 terms   */
        seq_t windowSum = cumSum - arr[0] - arr[1];   /* Σ a[2]..a[34] */

        for (idx_t i = 35; i <= n; ++i) {
            arr[i] = K41 * windowSum - K861 * arr[i - 34];
            /* update sliding window for next iteration   */
            windowSum += arr[i];
            windowSum -= arr[i - 33];
        }
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
