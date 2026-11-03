/*
 * A039734  a(n)=the prime q<prime(n) for which (prime(n) mod q) is maximal.
 * https://oeis.org/A039734
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the prime q < prime(n) for which (prime(n) mod q) is
 *            maximal Computation is performed by generating the first n
 *            primes, then scanning them to find the prime giving the largest
 *            remainder. Use a cache so repeated calls with arbitrary n are
 *            fast
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9710 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

static uint64_t *prime_cache = NULL;   /* array of primes */
static size_t prime_cache_size = 0;    /* number of primes stored */

/* compute the nth prime; extend cache if necessary */
static uint64_t nth_prime(idx_t n)
{
    if ((size_t)n > prime_cache_size) {
        /* shrink old cache */
        free(prime_cache);
        prime_cache = NULL;
        prime_cache_size = 0;

        /* estimate upper bound for nth prime and sieve up to it */
        size_t limit = _prime_limit_for_idx(n);
        build_primes_upto((idx_t)limit, &prime_cache, &prime_cache_size);
    }
    /* primes are 1‑based; array is 0‑based */
    return prime_cache[(size_t)(n - 1)];
}

seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;          /* undefined for n < 2, arbitrary */
    }

    /* obtain the nth prime */
    uint64_t p = nth_prime(n);

    uint64_t best_q = 0;
    uint64_t max_rem = 0;

    /* scan all primes < p */
    for (size_t i = 0; i + 1 < (size_t)n; ++i) {
        uint64_t q = prime_cache[i];
        uint64_t rem = p % q;
        if (rem > max_rem) {
            max_rem = rem;
            best_q = q;
        }
        /* tie: keep the first (= smallest) q producing max_rem */
    }

    return (seq_t)best_q;
}
