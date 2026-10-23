/*
 * A170265  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1 ≤ n ≤ 40: a(n)=2+14*∑_{k=0}^{n-1} a(k);
 *            a(41)=1+14*∑_{k=1}^{40} a(k)-105*a(0); for n ≥ 42:
 *            a(n)=14*∑_{k=1}^{40} a(n-k)-105*a(n-41);
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
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

    /* Allocate arrays for values and cumulative sums. */
    seq_t *arr   = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *pref  = (seq_t *)malloc((n + 1) * sizeof(seq_t));

    /* Base value a(0). */
    arr[0]  = (seq_t)1;
    pref[0] = (seq_t)1;

    /* Compute a(1) … a(min(n,40)). */
    idx_t lim = (n < 40 ? n : 40);
    for (idx_t i = 1; i <= lim; ++i)
    {
        /* a(i) = 2 + 14 * sum_{k=0}^{i-1} a(k).  sum_{k=0}^{i-1} a(k) = pref[i-1]. */
        arr[i] = (seq_t)(2 + (seq_t)14 * pref[i - 1]);
        pref[i] = pref[i - 1] + arr[i];
    }

    if (n >= 41)
    {
        /* a(41) = 1 + 14*sum_{k=1}^{40} a(k) - 105*a(0).  sum_{k=1}^{40} a(k) = pref[40] - a0. */
        seq_t sum_range = pref[40] - arr[0];
        arr[41] = (seq_t)(1 + (seq_t)14 * sum_range - (seq_t)105 * arr[0]);
        pref[41] = pref[40] + arr[41];

        /* Compute a(42) … a(n) using recurrence. */
        for (idx_t i = 42; i <= n; ++i)
        {
            /* sum_{k=1}^{40} a(i-k) = pref[i-1] - pref[i-41]. */
            seq_t s = pref[i - 1] - pref[i - 41];
            arr[i] = (seq_t)((seq_t)14 * s - (seq_t)105 * arr[i - 41]);
            pref[i] = pref[i - 1] + arr[i];
        }
    }

    seq_t result = arr[n];

    free(arr);
    free(pref);
    return result;
}
