/*
 * A121456  Expansion of q*(psi(-q)*psi(-q^3))^2 in powers of q where psi() is
 *          a Ramanujan theta function.
 * https://oeis.org/A121456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) is multiplicative. * a(1) = 1. * For a prime power p^e
 *            (e>0) * a(2^e) = -(2^e) * a(3^e) = 1 * a(p^e) = (p^(e+1) - 1) /
 *            (p - 1) for p > 3 * (the last is the sum 1 + p + … + p^e)
 * Range:     i8 n <= 109, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t ans = 1;
    idx_t m = n;

    /* factor 2 */
    idx_t e = 0;
    while (m % 2 == 0) {
        ++e;
        m /= 2;
    }
    if (e > 0) {
        seq_t f = -(((seq_t)1) << e);   /* -(2^e) */
        ans *= f;
    }

    /* factor 3 */
    e = 0;
    while (m % 3 == 0) {
        ++e;
        m /= 3;
    }
    /* a(3^e) = 1, so nothing to multiply */

    /* factor remaining primes > 3 */
    for (idx_t p = 5; p * p <= m; p += 2) {
        e = 0;
        while (m % p == 0) {
            ++e;
            m /= p;
        }
        if (e > 0) {
            /* compute sum 1 + p + p^2 + … + p^e */
            seq_t sum = 1;
            seq_t term = 1;
            for (idx_t i = 0; i < e; ++i) {
                term *= p;
                sum += term;
            }
            ans *= sum;
        }
    }

    if (m > 1) {   /* m is a prime > sqrt(original n) */
        /* e = 1 */
        seq_t sum = 1 + (seq_t)m;
        ans *= sum;
    }

    return ans;
}
