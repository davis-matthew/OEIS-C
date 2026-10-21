/*
 * A088693  E.g.f: A(x) = f(x*A(x)^2), where f(x) = (1+3*x)*exp(x).
 * https://oeis.org/A088693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] ((1+3*x)*exp(x))^(2*n+1)/(2*n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    /* k = 2*n + 1 */
    idx_t k = 2*n + 1;

    /* term_0 = k^(n-1) */
    seq_t sum = pow_int((seq_t)k, n - 1);
    seq_t term = sum;

    for (idx_t j = 0; j < n; ++j)
    {
        /* term_{j+1} = term_j * (k-j) * 3 * (n-j) / ((j+1)*k) */
        seq_t numer = term * (seq_t)(k - j);
        numer = numer * (seq_t)3;
        numer = numer * (seq_t)(n - j);
        seq_t denom = ((seq_t)(j + 1)) * k;
        term = numer / denom;      /* division is exact */
        sum += term;
    }

    return sum;
}
