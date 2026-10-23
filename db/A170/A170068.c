/*
 * A170068  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n ≥ 1: let p_n = 2 if 1 ≤ n ≤ 36 = 1 if n = 0 or n
 *            = 37 = 0 otherwise a(n) = p_n + 9 * Σ_{k=1}^{36} a(n-k) – 45 *
 *            a(n-37) (a(k) = 0 for k < 0, and indices are unsigned.)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate array for a[0..n] */
    seq_t *a = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!a) {
        return (seq_t)0;          /* allocation failed – return 0 */
    }

    a[0] = (seq_t)1;              /* a_0 */

    for (idx_t i = 1; i <= n; ++i) {
        /* sum of the last 36 terms a(i-1) ... a(i-36) */
        seq_t sum = 0;
        for (idx_t j = 1; j <= 36 && i >= j; ++j) {
            sum += a[i - j];
        }

        /* determine p_i */
        seq_t p_i;
        if (i <= 36)          /* 1 ≤ i ≤ 36 */
            p_i = (seq_t)2;
        else if (i == 37) {   /* i = 37 */
            p_i = (seq_t)1;
        } else {              /* i > 37 */
            p_i = (seq_t)0;
        }

        /* compute a[i] using the recurrence */
        seq_t term = p_i + (seq_t)9 * sum;
        if (i >= 37) {
            term -= (seq_t)45 * a[i - 37];
        }

        a[i] = term;
    }

    seq_t result = a[n];
    free(a);
    return result;
}
