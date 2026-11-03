/*
 * A139082  a(n) = (largest power of a prime dividing n) * (largest power of a
 *          prime dividing (n+1)).
 * https://oeis.org/A139082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A034699(n) * A034699(n+1) where A034699(k) is the largest
 *            prime power dividing k
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9939 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the largest prime power dividing a positive integer m.
   If m == 0, 0 is returned; for m == 1 the result is 1. */
static seq_t largest_prime_power(uint64_t m)
{
    if (m == 0)
        return (seq_t)0;

    seq_t max_pow = (seq_t)1;
    uint64_t n = m;

    /* trial divide by 2 first for efficiency */
    if ((n & 1ULL) == 0) {
        seq_t pow = (seq_t)1;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            pow *= (seq_t)2;
        }
        if (pow > max_pow)
            max_pow = pow;
    }

    /* now test odd divisors up to sqrt(n) */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            seq_t pow = (seq_t)1;
            while (n % p == 0) {
                n /= p;
                pow *= (seq_t)p;
            }
            if (pow > max_pow)
                max_pow = pow;
        }
    }

    /* the remaining n > 1 is a prime factor of exponent 1 */
    if (n > 1) {
        seq_t pow = (seq_t)n;
        if (pow > max_pow)
            max_pow = pow;
    }

    return max_pow;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined at offset 0; safe guard */
        return (seq_t)0;

    seq_t lp1 = largest_prime_power((uint64_t)n);
    seq_t lp2 = largest_prime_power((uint64_t)(n + 1));

    return lp1 * lp2;
}
