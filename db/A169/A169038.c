/*
 * A169038  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* Recurrence:
   a(0) = 1;
   for 1 ≤ n ≤ 23:  a(n) = 23*∑_{k=1}^{n} a(n−k) + 2;
   a(24) = 23*∑_{k=1}^{23} a(24−k) + 276*a(0) + 1;
   for n ≥ 25:      a(n) = 23*∑_{k=1}^{23} a(n−k) − 276*a(n−24). */
seq_t a(idx_t n)
{
    seq_t *a = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!a) return (seq_t)0;   /* should not happen */

    const seq_t ONE   = (seq_t)1;
    const seq_t TWO   = (seq_t)2;
    const seq_t C23   = (seq_t)23;
    const seq_t C276  = (seq_t)276;

    a[0] = ONE;
    if (n == 0) {
        seq_t r = a[0];
        free(a);
        return r;
    }

    /* compute a(1)..a(24) using the numerator coefficients */
    for (idx_t i = 1; i <= n && i <= 24; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= 23 && k <= i; ++k)
            sum += C23 * a[i - k];
        if (i == 24) {
            sum += C276 * a[0];
            a[i] = sum + ONE;   /* numerator coefficient 1 at i = 24 */
        } else {
            a[i] = sum + TWO;   /* numerator coefficient 2 for 1≤i≤23 */
        }
    }

    if (n <= 24) {
        seq_t r = a[n];
        free(a);
        return r;
    }

    /* compute a(25)..a(n) with the homogeneous recurrence */
    for (idx_t i = 25; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= 23; ++k)
            sum += C23 * a[i - k];
        sum -= C276 * a[i - 24];
        a[i] = sum;
    }

    seq_t r = a[n];
    free(a);
    return r;
}
