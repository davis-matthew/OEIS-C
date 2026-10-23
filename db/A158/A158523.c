/*
 * A158523  Moebius transform of negated primes in factorization of n.
 * https://oeis.org/A158523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = product over prime powers p^e || n of (-1)^e * (p+1) *
 *            p^(e-1), with a(1) = 1
 * Range:     i8 n <= 59, i16 n <= 7958, i32 n <= 7958, i64 n <= 7958,
 *            i128 n <= 7958, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, return 0 */
    if (n == 1) return (seq_t)1;

    uint64_t temp = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* handle factor 2 */
    if ((temp & 1ULL) == 0) {
        uint64_t exp = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;          /* divide by 2 */
            ++exp;
        }
        seq_t sign = (exp & 1ULL) ? (seq_t)-1 : (seq_t)1;
        seq_t base_plus = (seq_t)2 + 1;   /* 2 + 1 */
        seq_t pow2 = 1;
        for (uint64_t i = 0; i < exp - 1; ++i)
            pow2 *= (seq_t)2;                    /* 2^(exp-1) */
        result *= sign * base_plus * pow2;
    }

    /* handle odd prime factors up to sqrt(temp) */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t sign = (exp & 1ULL) ? (seq_t)-1 : (seq_t)1;
            seq_t base_plus = (seq_t)p + 1;
            seq_t powp = 1;
            for (uint64_t i = 0; i < exp - 1; ++i)
                powp *= (seq_t)p;                 /* p^(exp-1) */
            result *= sign * base_plus * powp;
        }
    }

    /* remaining prime factor, if any (exponent 1) */
    if (temp > 1) {
        seq_t sign = (seq_t)-1;            /* exponent = 1 → odd */
        seq_t base_plus = (seq_t)temp + 1;
        result *= sign * base_plus;        /* p^(1-1) = 1 */
    }

    return result;
}
