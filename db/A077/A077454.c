/*
 * A077454  a(n) = sigma_3(n^3)/sigma(n^3).
 * https://oeis.org/A077454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma_3(n^3)/sigma(n^3) = product_{p^e||n} (p^(6e+2) +
 *            p^(3e+1) + 1)/(p^2 + p + 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1626,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/int_sqrt.h"

/* static cache for primes */
static uint64_t *primes = NULL;            /* array of primes */
static size_t    prime_cnt = 0;            /* number of primes in the array */
static idx_t     prime_limit = 0;          /* highest limit for which primes are built */

/* fast exponentiation returning a 128‑bit value */
static __uint128_t pow_uint128(uint64_t base, unsigned exp)
{
    __uint128_t result = 1;
    __uint128_t b = base;
    while (exp > 0) {
        if (exp & 1)
            result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n <= 1)
        return (seq_t)1;

    /* Prepare prime list up to sqrt(n) */
    idx_t limit = int_sqrt(n);
    if (limit > prime_limit) {
        if (primes)
            free(primes);
        build_primes_upto(limit, &primes, &prime_cnt);
        prime_limit = limit;
    }

    idx_t m = n;
    __uint128_t result128 = 1;   /* use 128‑bit intermediate */
    for (size_t i = 0; i < prime_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            /* a(p^e) = (p^(6e+2) + p^(3e+1) + 1)/(p^2 + p + 1) */
            __uint128_t p128 = p;
            __uint128_t num = pow_uint128(p, 6 * e + 2) + pow_uint128(p, 3 * e + 1) + 1;
            __uint128_t den = pow_uint128(p, 2) + pow_uint128(p, 1) + 1;
            __uint128_t factor = num / den;   /* division exact */
            result128 *= factor;
        }
    }

    /* Remaining prime factor > sqrt(n) */
    if (m > 1) {
        uint64_t p = m;
        unsigned e = 1;
        __uint128_t num = pow_uint128(p, 6 * e + 2) + pow_uint128(p, 3 * e + 1) + 1;
        __uint128_t den = pow_uint128(p, 2) + pow_uint128(p, 1) + 1;
        __uint128_t factor = num / den;
        result128 *= factor;
    }

    return (seq_t)result128;
}
