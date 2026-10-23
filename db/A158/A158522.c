/*
 * A158522  Dirichlet inverse of number of unitary divisors of n (A034444).
 * https://oeis.org/A158522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = (-1)^Omega(n) * 2^omega(n) (Omega = total prime
 *            multiplicity, omega = number of distinct primes)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    uint64_t temp = n;          /* local copy of the argument */
    unsigned omega = 0;          /* distinct prime factors count */
    unsigned tot = 0;            /* total exponents (Omega) count */

    /* Trial division: p <= temp/p is equivalent to p*p <= temp
       and avoids overflow. */
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            ++omega;                 /* new distinct prime detected */
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;                /* count multiplicity */
            }
            tot += exp;              /* add to total count */
        }
    }

    if (temp > 1) {                /* remaining prime factor (exponent 1) */
        ++omega;
        ++tot;
    }

    seq_t val  = pow2((idx_t)omega);                /* 2^omega */
    seq_t sign = (tot & 1) ? (seq_t)-1 : (seq_t)1;  /* (-1)^tot */

    return sign * val;
}
