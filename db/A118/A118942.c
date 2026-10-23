/*
 * A118942  Primes p such that (p^2-13)/12 is prime.
 * https://oeis.org/A118942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 52, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that (p^2-13)/12 is prime */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"    /* is_prime_big(seq_t) */

/* --------------------------------------------------------------------- */
/* Prime sieve and sequence cache                                       */
static uint64_t _sieve_limit = 0;   /* current sieve upper bound         */
static uint64_t *_sieve_primes = NULL;  /* primes ≤ _sieve_limit          */
static size_t   _sieve_cnt    = 0;   /* number of primes collected       */
static size_t   _next_prime   = 0;   /* index of next prime to test      */

static seq_t   *_term_cache   = NULL;  /* sequence values                 */
static size_t   _term_cnt     = 0;      /* how many terms stored            */
static size_t   _term_cap     = 0;      /* capacity of cache array          */

/* --------------------------------------------------------------------- */
/* Build a prime list up to 'limit' (inclusive) */
static void _build_prime_list(uint64_t limit)
{
    /* Allocate a boolean sieve array (1 = prime, 0 = composite) */
    uint64_t *sieve = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    if (!sieve) { free(_sieve_primes); exit(1); }

    /* Initialize sieve: assume all entries are prime */
    for (uint64_t i = 0; i <= limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    /* Eratosthenes sweep */
    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (sieve[p]) {
            for (uint64_t q = p * p; q <= limit; q += p) {
                sieve[q] = 0;
            }
        }
    }

    /* Count primes so we can allocate the final array          */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;

    uint64_t *primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) { free(sieve); exit(1); }

    size_t j = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (sieve[i]) primes[j++] = i;
    }

    free(sieve);

    /* Replace the old prime list with the new one */
    free(_sieve_primes);
    _sieve_primes = primes;
    _sieve_cnt    = cnt;
    _sieve_limit  = limit;
    _next_prime   = 0;
}

/* --------------------------------------------------------------------- */
/* Ensure that we have at least `need` terms cached.  The cache grows
   as needed, rebuilding the prime list if we run out of primes.          */
static void _ensure_terms(idx_t need)
{
    while (_term_cnt < (size_t)need) {

        /* If we have exhausted the current prime list, double the limit */
        if (_next_prime >= _sieve_cnt) {
            uint64_t new_limit = (_sieve_limit == 0) ? 500000ULL
                                                     : _sieve_limit * 2;
            _build_prime_list(new_limit);
            continue;  /* restart loop with new list */
        }

        uint64_t p = _sieve_primes[_next_prime++];
        seq_t   q   = ((seq_t)p * (seq_t)p - (seq_t)13) / (seq_t)12;

        if (is_prime_big(q)) {
            /* Grow the cache if needed */
            if (_term_cap == 0) {
                _term_cap    = 128;
                _term_cache  = (seq_t *)malloc(_term_cap * sizeof(seq_t));
                if (!_term_cache) exit(1);
            } else if (_term_cnt >= _term_cap) {
                _term_cap   *= 2;
                _term_cache  = (seq_t *)realloc(_term_cache,
                        _term_cap * sizeof(seq_t));
                if (!_term_cache) exit(1);
            }
            _term_cache[_term_cnt++] = (seq_t)p;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Public sequence function required by the build system.                 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0; guard */
    _ensure_terms(n);
    return _term_cache[n - 1];
}
