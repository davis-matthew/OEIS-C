/*
 * A179549  Primes p such that p minus the sum of the square of its digits
 *          yields a prime.
 * https://oeis.org/A179549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = prime p such that p - (sum of squares of its decimal
 *            digits) is also a prime (strict difference). The sequence is
 *            indexed from 1
 * Range:     u8 n <= 1, u16 n <= 480, u32 n <= 654, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, digit_square_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/append_candidate.h"
#include "../utils/digit_square_sum.h"
#include "../utils/is_prime_big.h"

/* Static globals for lazy initialization */
static bool _initialized = false;
static uint8_t * _is_prime = NULL;      /* sieve bit array (0/1) */
static uint32_t * _prime_list = NULL;   /* list of primes up to LIMIT */
static size_t _prime_count = 0;         /* number of primes in the list */
static seq_t * _cache = NULL;           /* cached sequence values */
static size_t _cache_len = 0;
static size_t _cache_cap = 0;
static size_t _next_prime_idx = 0;      /* index in _prime_list to process next */

static void _build_sieve_and_primes(void) {
    /* Upper bound for primes needed to generate at least 10k valid terms.
       10 million is sufficient for the required range. */
    const size_t LIMIT = 10000000ull;

    /* Allocate sieve array */
    _is_prime = (uint8_t *)malloc(LIMIT + 1);
    if (_is_prime == NULL) abort();
    memset(_is_prime, 1, LIMIT + 1);
    _is_prime[0] = _is_prime[1] = 0;

    /* Sieve of Eratosthenes */
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (_is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i) {
                _is_prime[j] = 0;
            }
        }
    }

    /* Allocate prime list (≈ LIMIT / ln(LIMIT) ≈ LIMIT/10) */
    _prime_list = (uint32_t *)malloc((LIMIT / 10) * sizeof(uint32_t));
    if (_prime_list == NULL) abort();
    _prime_count = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (_is_prime[i]) {
            _prime_list[_prime_count++] = (uint32_t)i;
        }
    }

    /* Initialize cache */
    _cache = NULL;
    _cache_len = _cache_cap = 0;
    _next_prime_idx = 0;

    _initialized = true;
}

seq_t a(idx_t n) {
    /* Lazy initialization */
    if (!_initialized) {
        _build_sieve_and_primes();
    }

    /* Return cached value if already computed */
    if ((size_t)n <= _cache_len) {
        return _cache[(size_t)n - 1];
    }

    /* Compute terms until we reach n */
    while (_cache_len < (size_t)n) {
        /* Ensure we have more primes to test */
        if (_next_prime_idx >= _prime_count) {
            /* In practice this should never happen with the chosen LIMIT. */
            /* Return 0 to indicate failure. */
            return (seq_t)0;
        }

        uint32_t p = _prime_list[_next_prime_idx++];
        uint64_t sumsq = digit_square_sum(p);
        seq_t diff = (seq_t)p - (seq_t)sumsq;

        if (diff >= 2 && is_prime_big(diff)) {
            /* Store as next sequence term */
            append_candidate(&_cache, &_cache_len, &_cache_cap, (seq_t)p);
        }
    }

    return _cache[(size_t)n - 1];
}
