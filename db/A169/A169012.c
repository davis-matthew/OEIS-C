/*
 * A169012  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A169012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence derived from the generating function * * For 0 <= n
 *            <= 22: * a(0) = 1 * a(n) = 2 + 45 * (a(0)+a(1)+...+a(n-1)) * *
 *            For n >= 23: * a(n) = 45 * (a(n-1)+a(n-2)+...+a(n-22)) - 1035 *
 *            a(n-23) * * This follows from the rational generating function *
 *            A(t) = (1 + 2t + 2t^2 + ... + 2t^22 + t^23...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate an array of a[0..n] */
    seq_t *a = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!a)
        return (seq_t)0;   /* out of memory – unlikely in test harness */

    /* Base term a(0) = 1 */
    a[0] = (seq_t)1;

    /* Compute a[1]..a[22] using the first part of the recurrence */
    if (n >= 1) {
        seq_t cum = a[0];  /* sum of a[0..i-1] for current i */
        idx_t limit = (n < 22) ? (idx_t)n : 22;
        for (idx_t i = 1; i <= limit; ++i) {
            a[i] = (seq_t)2 + (seq_t)45 * cum;
            cum += a[i];
        }
    }

    /* Compute a[23]..a[n] using the second part of the recurrence */
    if (n >= 23) {
        /* Initialise window sum of a[1]..a[22] */
        seq_t window_sum = (seq_t)0;
        for (idx_t i = 1; i <= 22; ++i)
            window_sum += a[i];

        for (idx_t i = 23; i <= n; ++i) {
            /* a[i] = 45 * window_sum - 1035 * a[i-23] */
            a[i] = (seq_t)45 * window_sum - (seq_t)1035 * a[i - 23];

            /* Update window sum for the next iteration:
             * remove a[i-22] and add a[i] */
            window_sum += a[i];
            window_sum -= a[i - 22];
        }
    }

    seq_t result = a[n];
    free(a);
    return result;
}
