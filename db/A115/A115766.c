/*
 * A115766  Distance from the n-th prime to the nearest semiprime.
 * https://oeis.org/A115766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   This function returns the distance from the n‑th prime to * the
 *            nearest semiprime. It searches outward from the prime and *
 *            tests each candidate for being a product of exactly two (not *
 *            necessarily distinct) primes
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/_floor_isqrt_u64.h"

/* Cached list of primes used for trial division in is_semiprime(). */
static uint64_t *prime_list    = NULL;  /* array of primes */
static size_t   prime_count   = 0;     /* number of primes in the list     */
static idx_t    prime_limit   = 0;     /* maximum n such that primes <= prime_limit */

/* Rebuild the cached prime list to include all primes <= limit. */
static void
ensure_primes_upto(idx_t limit)
{
    if (prime_limit >= limit)
        return;                     /* already have enough primes */

    free(prime_list);
    prime_list  = NULL;
    prime_limit = 0;
    prime_count = 0;

    /* build_primes_upto allocates an array of primes pb and stores the
       number of primes in cnt. */
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* Test whether x is a semiprime (product of exactly two prime factors,
 * counting multiplicity).  The routine uses trial division with all
 * primes <= sqrt(x). */
static bool
is_semiprime(uint64_t x)
{
    if (x < 4U)            /* 4 is the smallest semiprime */
        return false;

    /* Ensure we have all primes up to sqrt(x). */
    idx_t need = _floor_isqrt_u64(x);
    if (need > prime_limit)
        ensure_primes_upto(need);

    uint64_t  remaining = x;
    unsigned  total     = 0;            /* total number of prime factors counted with multiplicity */

    for (size_t i = 0; i < prime_count && prime_list[i] <= need; ++i) {
        uint64_t p = prime_list[i];
        while (remaining % p == 0ULL) {
            ++total;
            remaining /= p;
            if (total > 2)            /* early exit */
                return false;
        }
    }
    /* If anything is left, it is a prime factor > sqrt(x). */
    if (remaining > 1ULL)
        ++total;

    return (total == 2);
}

/* Main sequence accessor */
seq_t
a(idx_t n)
{
    /* n is 1-based (OEIS offset 1). */
    uint64_t p = get_prime(n);

    for (uint64_t diff = 1ULL; ; ++diff) {
        /* Check downward side only if it stays ≥ 2. */
        if (p > diff) {
            uint64_t down = p - diff;
            if (is_semiprime(down))
                return (seq_t)diff;
        }

        /* Check upward side. */
        uint64_t up = p + diff;
        if (is_semiprime(up))
            return (seq_t)diff;
    }
}
