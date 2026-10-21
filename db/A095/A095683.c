/*
 * A095683  Number of prime power divisors of n. If n = product p_i^r_i then d
 *          = product {p_i^s_i, 2 <= s_i <= r_i, s_i is prime} is a prime
 *          power divisor of n.
 * https://oeis.org/A095683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e || n} Pi(e) where Pi(e) is the number of
 *            primes <= e (prime counting function). Thus a(p^e) = Pi(e) and
 *            a(n) is multiplicative. If any exponent e <= 1 then Pi(e)=0 and
 *            the whole product is 0. This follows the OEIS definition of
 *            prime‑power divisors
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Precomputed prime counts Pi(e) for 0 <= e <= 64 */
static const uint8_t prime_count[65] = {
    0, 0, 1, 2, 2, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, /* 0-15 */
    6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, /* 16-31 */
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, /* 32-47 */
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, /* 48-63 */
    8                                            /* 64 */
};

/* Simple prime test for small numbers */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Return the number of prime power divisors of n. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* a(1) = 1 by definition */

    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    /* Handle factor 2 */
    if ((m & 1ULL) == 0) {
        uint8_t exp = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++exp;
        }
        uint8_t k = prime_count[exp];
        if (k == 0) return (seq_t)0;
        result *= (seq_t)k;
    }

    /* Handle odd factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint8_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            uint8_t k = prime_count[exp];
            if (k == 0) return (seq_t)0;
            result *= (seq_t)k;
        }
    }

    /* Remaining prime factor (if any) has exponent 1 -> k=0 */
    if (m > 1) {
        /* exponent is 1 => prime_count[1] = 0 */
        return (seq_t)0;
    }

    return result;
}
