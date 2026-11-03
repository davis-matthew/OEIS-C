/*
 * A023900  Dirichlet inverse of Euler totient function (A000010).
 * https://oeis.org/A023900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice, mult
 * Formula:   a(n) = Product_{p|n} (1-p) (Dirichlet inverse of Euler's
 *            totient)
 * Range:     i8 n <= 130, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 97 terms through n = 9387 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n = 1 – the empty product is 1. */
    if (n == 1)
        return (seq_t)1;

    /* Accumulate the product over distinct prime divisors of n. */
    seq_t result = (seq_t)1;
    idx_t m = n;

    /* Handle the prime factor 2 separately, so the rest of the loop
       can step by 2 over odd numbers. */
    if ((m & (idx_t)1) == 0) {
        /* (1 - 2) = -1 */
        result *= ((seq_t)1 - (seq_t)2);
        while ((m & (idx_t)1) == 0)
            m >>= 1u;            /* strip all powers of 2 */
    }

    /* Scan odd divisors up to sqrt(m). */
    for (idx_t p = (idx_t)3; p <= m / p; p += (idx_t)2) {
        if (m % p == 0) {
            /* Multiply by (1 - p) once per distinct prime factor. */
            result *= ((seq_t)1 - (seq_t)p);
            while (m % p == 0)
                m /= p;          /* remove all copies of the same prime */
        }
    }

    /* If a prime factor larger than sqrt(n) remains, multiply it. */
    if (m > (idx_t)1)
        result *= ((seq_t)1 - (seq_t)m);

    return result;
}
