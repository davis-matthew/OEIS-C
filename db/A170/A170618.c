/*
 * A170618  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence satisfies the linear recurrence a(n) = 31 *
 *            (a(n-1)+a(n-2)+…+a(n-47)) - 496 * a(n-48) with initial values
 *            a(0)=1, a(1)=33, a(2)=1056, …, a(48)=..., obtained from the
 *            generating function numerator: 1 + 2t + 2t^2 + … + 2t^47 + t^48
 *            denominator: 1 - 31t - 31t^2 - … - 31t^47 + 496t^48
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate array [0..n] */
    seq_t *a = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    a[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum47 = (seq_t)0;
        for (idx_t k = 1; k <= 47; ++k) {
            if (i >= k) {
                sum47 += a[i - k];
            }
        }

        /* b_i: numerator coefficient of t^i */
        seq_t term = (seq_t)0;
        if (i == 48) {
            term = (seq_t)1;
        } else if (i >= 1 && i <= 47) {
            term = (seq_t)2;
        } /* for i > 48 term stays 0 */

        seq_t sub = (seq_t)0;
        if (i >= 48) {
            sub = a[i - 48];
        }

        a[i] = term + (seq_t)31 * sum47 - (seq_t)496 * sub;
    }

    seq_t res = a[n];
    free(a);
    return res;
}
