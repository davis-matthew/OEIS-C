/*
 * A189244  The n-th derivative of e^((2-x-x^2)/(1-x-x^2)), evaluated at x=1.
 * https://oeis.org/A189244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n!*sum(m=1..n, sum(k=m..n, binomial(k-1,m-1) *
 *            binomial(k,n-k) * (-1)^(m+k)*3^(2*k-n)/m!), a(0)=1. Recurrence
 *            derived from the differential equation (1+6z+11z^2+6z^3+z^4)
 *            f'(z) = (3+2z) f(z) with f(z)=∑ a(n)/n! z^n. This gives a_{m+1}
 *            = (3-6m) a_m + (2-11(m-1)) m a_{m-1} - 6(m-2) m(m-1) a_...
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;
    if (n == 2)
        return (seq_t)-7;

    seq_t *A = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!A)
        return (seq_t)0;   /* Out‑of‑memory fallback */

    A[0] = (seq_t)1;
    A[1] = (seq_t)3;
    A[2] = (seq_t)-7;

    for (idx_t m = 2; m <= n - 1; ++m)         /* compute a_{m+1} */
    {
        seq_t term1 = ((seq_t)3 - (seq_t)6 * (seq_t)m) * A[m];

        seq_t coeff2 = ((seq_t)2 - (seq_t)11 * (seq_t)(m - 1));
        seq_t term2 = coeff2 * (seq_t)m * A[m - 1];

        seq_t term3 = -(seq_t)6 * (seq_t)(m - 2) * (seq_t)m * (seq_t)(m - 1) * A[m - 2];

        seq_t term4 = -(seq_t)(m - 3) * (seq_t)m * (seq_t)(m - 1) *
                      (seq_t)(m - 2) * A[m - 3];

        A[m + 1] = term1 + term2 + term3 + term4;
    }

    seq_t result = A[n];
    free(A);
    return result;
}
