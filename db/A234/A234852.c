/*
 * A234852  Indices k of primes p=prime(k) such that prime(p)-p+1 is prime.
 * https://oeis.org/A234852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the index k such that p = prime(k) and prime(p) – p + 1
 *            is prime
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   build_primes_upto, ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes a cache of qualifying indices using a sieve of primes
   up to a fixed limit.  The limit is chosen large enough to provide more than 10 000
   terms.  The cache is filled on the first call. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

static bool _initialized = false;
static uint64_t * _primes = NULL;          /* array of primes */
static size_t    _prime_count = 0;        /* number of primes in _primes */
static seq_t *   _cache = NULL;           /* cache of qualifying indices k */
static size_t    _cache_len = 0;           /* number of stored indices */
static size_t    _cache_cap = 0;           /* allocated capacity for _cache */

/* Fill the cache with qualifying indices k up to a fixed sieve limit. */
static void _init_cache(void)
{
    if (_initialized) return;

    /* About 2 000 000 can be generated quickly and gives far more than 10 000
       qualifying indices, enough for the required tests. */
    const idx_t LIMIT = 2000000ULL;
    build_primes_upto(LIMIT, &_primes, &_prime_count);

    _cache_cap = 1024;
    _cache = (seq_t *)malloc(_cache_cap * sizeof(seq_t));

    for (size_t i = 0; i < _prime_count; ++i) {
        uint64_t pk = _primes[i];                     /* prime(k), k = i+1 */
        if (pk == 0) continue;                        /* safety */
        /* pk is the index of the next prime we need.  We must have pk <= prime_count. */
        if (pk > _prime_count) break;

        uint64_t prime_of_pk = _primes[pk - 1];        /* prime(pk) */
        idx_t    t = (idx_t)prime_of_pk - pk + 1;       /* prime(pk) - pk + 1 */

        if (t >= 2 && is_prime(t)) {
            if (_cache_len == _cache_cap)
                ensure_capacity(_cache_len + 1, &_cache_cap,
                                (void **)&_cache, sizeof(seq_t));
            _cache[_cache_len++] = (seq_t)(i + 1);     /* k is 1‑based */
        }
    }
    _initialized = true;
}

seq_t a(idx_t n)
{
    _init_cache();
    if (n == 0 || (size_t)n > _cache_len) return 0;
    return _cache[n - 1];
}
