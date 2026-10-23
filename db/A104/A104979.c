/*
 * A104979  Semidiagonal sums of triangle A104978: a(n) = Sum_{k=0..[n/2]}
 *          A104978(n-k,n-2*k).
 * https://oeis.org/A104979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1; For n≥2: a(n) = Σ_{i=0}^{n-2} a(i)*a(n-2-i) +
 *            Σ_{i=0}^{n-1} a(i)*b(n-1-i) where b(k) = Σ_{j=0}^{k}
 *            a(j)*a(k-j). This follows from the GF A(x)=1+x*A^3+x^2*A^2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    /* Allocate arrays for coefficients a[0..n] and temp[0..n] */
    seq_t *a_coef = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *temp    = (seq_t *)malloc((n + 1) * sizeof(seq_t));

    a_coef[0] = (seq_t)1;
    temp[0]   = (seq_t)1;  /* b(0) = a(0)*a(0) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t pair_sum = 0;
        if (i >= 2) {
            for (idx_t j = 0; j <= i - 2; ++j)
                pair_sum += a_coef[j] * a_coef[i - 2 - j];
        }

        seq_t triple_sum = 0;
        for (idx_t t = 0; t <= i - 1; ++t)
            triple_sum += a_coef[t] * temp[i - 1 - t];

        a_coef[i] = pair_sum + triple_sum;

        /* Compute temp[i] = b(i) = Σ_{j=0}^{i} a(j)*a(i-j) */
        seq_t sum = 0;
        for (idx_t j = 0; j <= i; ++j)
            sum += a_coef[j] * a_coef[i - j];
        temp[i] = sum;
    }

    seq_t result = a_coef[n];
    free(a_coef);
    free(temp);
    return result;
}
