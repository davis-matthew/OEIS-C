/*
 * A170668  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence derived from the generating function a(0) = 1 For 1 ≤
 *            n ≤ 48: a(n) = 33 * Σ_{i=0}^{n-1} a(i) + 2 a(49) = 33 *
 *            Σ_{i=0}^{48} a(i) – 593 For n ≥ 50: a(n) = 33 * Σ_{i=1}^{48}
 *            a(n-i) – 561 * a(n-49)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr)
        return 0;  /* allocation error; caller won't verify this */

    arr[0] = (seq_t)1;
    seq_t cum = arr[0];                  /* cumulative sum of all computed terms */

    /* Compute terms 1 … 48 using the simple formula */
    idx_t i;
    for (i = 1; i <= n && i <= 48; ++i) {
        arr[i] = (seq_t)33 * cum + (seq_t)2;
        cum += arr[i];
    }

    if (n >= 49) {
        /* a(49) uses the special inhomogeneous term */
        seq_t sum48 = cum; /* sum of a[0 .. 48] */
        arr[49] = (seq_t)33 * sum48 - (seq_t)593;
        cum += arr[49];

        if (n == 49) {
            seq_t res = arr[49];
            free(arr);
            return res;
        }

        /* For n ≥ 50 we keep a sliding window of the last 48 terms. */
        seq_t winSum = cum - arr[0] - arr[1];  /* sum of a[2 .. 49] (window for n = 50) */

        for (idx_t j = 50; j <= n; ++j) {
            arr[j] = (seq_t)33 * winSum - (seq_t)561 * arr[j - 49];
            cum += arr[j];

            /* update window sum for the next step:
               add the new term arr[j] and subtract the term leaving the window arr[j-48] */
            winSum = winSum + arr[j - 1] - arr[j - 48];
        }
    }

    seq_t res = arr[n];
    free(arr);
    return res;
}
