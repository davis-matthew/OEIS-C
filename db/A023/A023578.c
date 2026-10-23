/*
 * A023578  Least odd prime divisor of prime(n)+3, or 1 if prime(n)+3 is a
 *          power of 2.
 * https://oeis.org/A023578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least odd prime divisor of prime(n)+3, or 1 if prime(n)+3
 *            is a power of 2
 * Range:     u8 n <= 30, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt, get_prime, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/is_power_of_two.h"
#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"

/* Cache of primes used for factorisation */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_len = 0;
static uint64_t prime_cache_limit = 0;   /* highest prime included in the cache */

/* Smallest odd prime factor of an odd number v (v > 1) */
static uint64_t smallest_odd_prime_factor(uint64_t v)
{
    /* v is odd here */
    uint64_t limit = (uint64_t)floor_sqrt((seq_t)v);   /* sqrt(v) as uint64_t */

    /* Ensure the prime cache covers all primes up to `limit` */
    if (limit > prime_cache_limit) {
        if (prime_cache) free(prime_cache);
        build_primes_upto(limit, &prime_cache, &prime_cache_len);
        prime_cache_limit = limit;
    }

    for (size_t i = 0; i < prime_cache_len; ++i) {
        uint64_t p = prime_cache[i];
        if (p == 2) continue;       /* skip factor 2 – v is already odd */
        if (p > limit) break;
        if (v % p == 0) return p;
    }
    /* v itself is prime */
    return v;
}

seq_t a(idx_t n)
{
    uint64_t p   = get_prime(n);          /* nth prime (1‑indexed) */
    uint64_t v   = p + 3ULL;              /* value to factor */

    if (is_power_of_two(v))
        return (seq_t)1;                  /* special case */

    /* remove any powers of 2, leaving the odd part */
    while ((v & 1ULL) == 0)
        v >>= 1;

    uint64_t sf = smallest_odd_prime_factor(v);   /* least odd prime divisor */
    return (seq_t)sf;
}
