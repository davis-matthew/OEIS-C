/*
 * A127067  Define an array by d(m, 0) = 1, d(m, 1) = m; d(m, k) = (m - k + 1)
 *          d(m+1, k-1) - (k-1) (m+1) d(m+2, k-2). Sequence gives d(2,n).
 * https://oeis.org/A127067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = d(2,n) where d(m,0)=1, d(m,1)=m, d(m,k)=
 *            (m-k+1)d(m+1,k-1)-(k-1)(m+1)d(m+2,k-2). Along the antidiagonal
 *            m+k = n+2 we define f(k)=d(n+2-k,k). Initial values: f(0)=1,
 *            f(1)=n+1. Recurrence: f(k) = (S-2k+1)f(k-1) -
 *            (k-1)(S-k+1)f(k-2), S=n+2
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 19, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    size_t sz = (size_t)n + 1;
    seq_t *f = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!f)
        return 0; /* allocation failed – unlikely */

    seq_t S = (seq_t)n + (seq_t)2;
    f[0] = (seq_t)1;
    f[1] = S - (seq_t)1;    /* n+1 */

    for (idx_t k = 2; k <= n; ++k) {
        /* (S - 2k + 1) */
        seq_t factor1 = S - (seq_t)(2 * k) + (seq_t)1;
        seq_t term1   = factor1 * f[k - 1];

        /* (k-1)(S - k + 1) */
        seq_t factor2 = ((seq_t)(k - 1)) * (S - (seq_t)k + (seq_t)1);
        seq_t term2   = factor2 * f[k - 2];

        f[k] = term1 - term2;
    }

    seq_t result = f[n];
    free(f);
    return result;
}
