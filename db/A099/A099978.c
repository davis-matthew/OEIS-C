/*
 * A099978  Bisection of A001157: a(n) = sigma_2(2n-1).
 * https://oeis.org/A099978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma_2(2*n-1) where sigma_2(m) is the sum of the squares
 *            of all divisors of m
 * Range:     u8 n <= 7, u16 n <= 121, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2*n-1 is odd, compute its sum of squares of divisors. */
    uint64_t m = 2 * n - 1;          /* value fits in 64‑bit for the intended range */
    uint64_t temp = m;
    seq_t result = 1;                /* multiplicative identity */

    /* trial division by odd numbers up to sqrt(temp) */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            seq_t pow = 1;           /* p^0 initially */
            seq_t sigma = 1;         /* 1 + p^2 + p^4 + ... */

            /* factor p out of temp and accumulate sigma contribution */
            while (temp % p == 0) {
                temp /= p;
                pow *= (seq_t)p;     /* p^k */
                sigma += pow * pow;  /* add p^{2k} */
            }

            result *= sigma;         /* multiply contributions of distinct primes */
        }
    }

    /* a remaining prime factor > sqrt(original m) remains in temp */
    if (temp > 1) {
        seq_t t = (seq_t)temp;
        seq_t sigma = 1 + t * t;     /* exponent is 1: 1 + p^2 */
        result *= sigma;
    }

    return result;
}
