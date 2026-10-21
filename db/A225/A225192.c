/*
 * A225192  Number of primes p such that p is -1 mod n where p < n-th prime.
 * https://oeis.org/A225192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p < prime(n) with p ≡ -1 (mod n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_cache = NULL;   /* array of primes up to prime_limit */
static size_t prime_cnt = 0;           /* number of primes stored */
static idx_t  prime_limit = 0;         /* upper bound used to generate prime_cache */
static idx_t  max_cached_n = 0;        /* largest n for which cache is guaranteed */

/* Ensure that prime_cache contains at least the first n primes.
   If necessary, rebuild the cache with a larger upper bound. */
static void
ensure_primes(idx_t n)
{
    if (n <= max_cached_n)
        return;                 /* cache already sufficient */

    /* Try to use the recommended upper bound for the n-th prime */
    idx_t new_limit = _prime_limit_for_idx(n);
    if (new_limit <= prime_limit && prime_cnt >= n) {
        max_cached_n = n;
        return;
    }

    /* Rebuild cache with a bound large enough */
    if (prime_cache)
        free(prime_cache);
    size_t new_cnt;
    build_primes_upto(new_limit, &prime_cache, &new_cnt);
    prime_cnt  = new_cnt;
    prime_limit = new_limit;

    /* Safety: if estimation was too low, double until we have enough primes */
    while (prime_cnt < n) {
        new_limit *= 2;
        free(prime_cache);
        build_primes_upto(new_limit, &prime_cache, &new_cnt);
        prime_cnt  = new_cnt;
        prime_limit = new_limit;
    }

    max_cached_n = n;
}

seq_t a(idx_t n)
{
    /* n >= 1 by OEIS offset */
    ensure_primes(n);

    /* nth prime is stored at index n-1 (0‑based) */
    uint64_t nth_prime = prime_cache[n - 1];
    seq_t count = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p >= nth_prime)
            break;      /* all further primes are too large */
        if (p % n == n - 1)
            ++count;
    }
    return count;
}
