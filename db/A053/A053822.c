/*
 * A053822  Dirichlet inverse of sigma_2 function (A001157).
 * https://oeis.org/A053822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, look
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * mu(d) * d^2 * (Dirichlet inverse of
 *            sigma_2, multiplicative with * a(p) = -(1 + p^2), a(p^2) = p^2,
 *            a(p^e) = 0 for e>=3)
 * Range:     i8 n <= 9, i16 n <= 165, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;            /* a(1) = 1 */

    seq_t result = 1;                /* product of local factors */
    idx_t tmp    = n;                /* factorization work variable */

    /* factor 2 */
    idx_t e = 0;
    while ((tmp & 1ULL) == 0) {      /* tmp % 2 == 0 */
        tmp >>= 1;
        e++;
    }
    if (e > 0) {
        if (e >= 3) return 0;        /* zero factor kills the whole product */
        if (e == 1) {
            seq_t p2 = (seq_t)4;    /* 2*2 */
            result *= -( (seq_t)1 + p2 );
        } else { /* e == 2 */
            seq_t p2 = (seq_t)4;
            result *= p2;
        }
    }

    /* odd primes */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            if (e >= 3) return 0;
            seq_t p2 = (seq_t)p * (seq_t)p;
            if (e == 1)     result *= -( (seq_t)1 + p2 );
            else            result *= p2;     /* e == 2 */
        }
    }

    /* remaining prime factor (exponent 1) */
    if (tmp > 1) {
        seq_t p  = (seq_t)tmp;
        seq_t p2 = p * p;
        result *= -( (seq_t)1 + p2 );
    }

    return result;
}
