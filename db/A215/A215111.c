/*
 * A215111  Numbers k such that k^2+1 divides k!!.
 * https://oeis.org/A215111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 5179, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _legendre_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by searching for the n‑th even integer k such that
   k^2+1 divides (k/2)!.  The function keeps a cache of found terms
   and a pointer to the next candidate to test. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_legendre_factorial.h"

/* ---------- prime sieve up to a fixed limit ----------
   Sieve is performed once on first call.  LIMIT has to be
   larger than any k that might appear for the requested n. */
static bool _sieve_done = false;
static uint32_t * _primes = NULL;       /* array of all primes ≤ LIMIT */
static size_t     _prime_cnt = 0;       /* number of primes stored */
static const uint32_t LIMIT = 2000000;  /* enough for n up to about 10 000 */

/* Checks whether k (even) satisfies (k/2)! % (k^2+1) == 0. */
static bool _check_candidate(idx_t k)
{
    idx_t m = k / 2;
    unsigned __int128 prod = (unsigned __int128)k * k + 1; /* k^2+1 */
    uint64_t n = (uint64_t)prod;   /* fits for all k we examine */
    uint64_t tmp = n;

    /* Trial division using precomputed primes. */
    for (size_t i = 0; i < _prime_cnt && (uint64_t)_primes[i] * _primes[i] <= tmp; ++i) {
        uint32_t p = _primes[i];
        if (tmp % p == 0) {
            int exp_p = 0;
            do { tmp /= p; ++exp_p; } while (tmp % p == 0);
            if (_legendre_factorial(m, p) < (seq_t)exp_p) return false;
        }
    }
    if (tmp > 1) {                /* remaining prime factor */
        uint32_t p = (uint32_t)tmp;
        if (_legendre_factorial(m, p) < 1) return false;
    }
    return true;
}

/* ---------- memoised sequence storage ----------
   Keeps the found terms and the next candidate to test. */
static seq_t  * _cache = NULL;
static size_t  _cap   = 0;
static size_t  _size  = 0;
static idx_t   _next_candidate = 2;
static bool    _initialised = false;

/* ---------- public entry function ----------
   Returns the n‑th term of A215111.  */
seq_t a(idx_t n)
{
    /* Initialise prime sieve on first call. */
    if (!_sieve_done) {
        bool *marked = (bool*)calloc(LIMIT + 1, sizeof(bool));
        for (uint32_t i = 2; i * i <= LIMIT; ++i)
            if (!marked[i])
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    marked[j] = true;

        size_t primes_cap = 100000;
        _primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
        for (uint32_t i = 2; i <= LIMIT; ++i)
            if (!marked[i]) {
                if (_prime_cnt == primes_cap) {
                    primes_cap <<= 1;
                    _primes = (uint32_t*)realloc(_primes,
                                                primes_cap * sizeof(uint32_t));
                }
                _primes[_prime_cnt++] = i;
            }
        free(marked);
        _sieve_done = true;
    }

    if (!_initialised) {
        _initialised = true;
        _next_candidate = 2;     /* start from the smallest even integer */
    }

    /* Generate terms up to the requested index. */
    while (_size < (size_t)n) {
        if (_next_candidate & 1U)            /* make it even  */
            ++_next_candidate;

        if (_check_candidate(_next_candidate)) {
            if (_size == _cap) {               /* enlarge cache  */
                size_t new_cap = _cap == 0 ? 8 : _cap << 1;
                _cache = (seq_t*)realloc(_cache,
                                         new_cap * sizeof(seq_t));
                _cap = new_cap;
            }
            _cache[_size++] = (seq_t)_next_candidate;
        }
        _next_candidate += 2;                  /* next even integer */
    }
    return _cache[n - 1];
}
