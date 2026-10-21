/*
 * A227899  Number of primes p < n with 3*p - 4 and n^2 + (n - p)^2 both
 *          prime.
 * https://oeis.org/A227899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p < n such that 3*p-4 is prime * and n^2
 *            + (n-p)^2 is prime
 * Range:     u8 n <= 15, u16 n <= 316, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/* cached list of primes up to prime_cache_limit */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static idx_t prime_cache_limit = 0;

/* Ensure that prime_cache contains all primes <= upper_limit. */
static void ensure_prime_cache(idx_t upper_limit)
{
    if (upper_limit <= prime_cache_limit)
        return;
    free(prime_cache);
    build_primes_upto(upper_limit, &prime_cache, &prime_cache_cnt);
    prime_cache_limit = upper_limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t cnt = 0;

    if (n <= 2)      /* no primes < n */
        return 0;

    ensure_prime_cache(n - 1);

    for (size_t i = 0; i < prime_cache_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p >= n) break;

        /* Check 3*p - 4 is prime. */
        if (!is_prime(3 * p - 4))
            continue;

        /* Compute n^2 + (n-p)^2. */
        idx_t np = (idx_t)(n - p);
        seq_t sq1 = (seq_t)n * n;
        seq_t sq2 = (seq_t)np * np;
        idx_t t2 = (idx_t)(sq1 + sq2);

        if (is_prime(t2))
            ++cnt;
    }
    return cnt;
}
