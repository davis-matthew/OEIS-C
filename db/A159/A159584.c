/*
 * A159584  Coefficients in expansion of eta(2z)^3*eta(6z)^3/theta(3z).
 * https://oeis.org/A159584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^(n-1) in eta(q^2)^3 * eta(q^3)^2 *
 *            eta(q^12)^2 / eta(q^6)^2 The product is ∏_{k≥1} (1 - q^k)^{e(k)}
 *            with e(k) = 5 if k ≡ 0 (mod 12) = 3 if k ≡ 6 (mod 12) = 2 if k ≡
 *            3 (mod 12) = 3 if k ≡ 2 (mod 12) = 0 otherwise
 * Range:     i8 n <= 63, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
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
        return (seq_t)0;           /* undefined for n=0 */

    idx_t N = n - 1;                 /* coefficient of q^N */
    seq_t *coeff = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!coeff)
        return (seq_t)0;            /* allocation failed */

    coeff[0] = (seq_t)1;             /* constant term */

    for (idx_t k = 1; k <= N; ++k)
    {
        int e;
        if (k % 12 == 0)       e = 5;
        else if (k % 6 == 0)   e = 3;
        else if (k % 3 == 0)   e = 2;
        else if (k % 2 == 0)   e = 3;
        else                   e = 0;

        for (int t = 0; t < e; ++t)
        {
            for (idx_t j = N; j >= k; --j)
                coeff[j] -= coeff[j - k];
        }
    }

    seq_t result = coeff[N];
    free(coeff);
    return result;
}
