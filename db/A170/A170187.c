/*
 * A170187  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1≤n<39: a(n)=2+32*∑_{k=0}^{n-1}a(k); for n≥39:
 *            a(n)=32*∑_{k=1}^{38}a(n‑k)−528*a(n‑39)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate a temporary array of size n+1 */
    seq_t *a = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (a == NULL) return 0;   /* abort on allocation failure */

    /* initial terms a[0] … a[38] */
    a[0] = (seq_t)1;
    seq_t sum_prev = a[0];            /* sum of all a[0 … i-1] for current i */

    idx_t i;
    for (i = 1; i < 39 && i <= n; ++i) {
        a[i] = (seq_t)2 + (seq_t)32 * sum_prev;
        sum_prev += a[i];
    }

    if (n < 39) {
        seq_t result = a[n];
        free(a);
        return result;
    }

    /* compute terms from a[39] up to a[n] using the linear recurrence */
    seq_t sum_last38 = 0;
    for (i = 1; i <= 38; ++i) sum_last38 += a[i];

    for (i = 39; i <= n; ++i) {
        a[i] = (seq_t)32 * sum_last38 - (seq_t)528 * a[i - 39];
        /* update sliding window sum of the last 38 terms */
        sum_last38 = sum_last38 + a[i] - a[i - 38];
    }

    seq_t result = a[n];
    free(a);
    return result;
}
