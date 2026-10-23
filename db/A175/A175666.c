/*
 * A175666  Sum of Sophie Germain prime p and corresponding safe prime q=2p+1.
 * https://oeis.org/A175666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * p_n + 1 * where p_n is the nth Sophie Germain prime,
 *            i.e. a prime p such that * 2p+1 is also prime. The sequence is
 *            the sum of p and its corresponding * safe prime q = 2p+1. a(n)
 *            is defined for n ≥ 1. This implementation * generates Sophie
 *            Germain primes lazily, storing them in a stati...
 * Range:     u8 n <= 9, u16 n <= 353, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------------- */
/*  Prime generation utilities                                              */
/* --------------------------------------------------------------------------- */

static size_t _prime_cap = 0;
static size_t _prime_count = 0;
static uint64_t * _primes = NULL;      /* list of primes, increasing order */

/* Append a new prime to the array, resizing if necessary */
static void _prime_append(uint64_t p) {
    if (_prime_count == _prime_cap) {
        size_t new_cap = (_prime_cap == 0) ? 32 : _prime_cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(_primes, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        _primes = tmp;
        _prime_cap = new_cap;
    }
    _primes[_prime_count++] = p;
}

/* Ensure that _primes contains all primes up to and including 'limit' */
static void _generate_primes_up_to(uint64_t limit) {
    static uint64_t _last_checked = 1;   /* numbers <= last_checked have been tested */
    if (limit <= _last_checked) return;

    for (uint64_t n = _last_checked + 1; n <= limit; ++n) {
        /* Test n for primality using already known primes */
        bool is_p = true;
        if (n < 2) {
            is_p = false;
        } else if (n == 2) {
            /* prime */
        } else if ((n & 1ULL) == 0) {
            is_p = false;
        } else {
            for (size_t i = 0; i < _prime_count; ++i) {
                uint64_t q = _primes[i];
                /* stop if q*q > n */
                __uint128_t prod = (__uint128_t)q * (__uint128_t)q;
                if (prod > n) break;
                if (n % q == 0) {
                    is_p = false;
                    break;
                }
            }
        }
        if (is_p) _prime_append(n);
    }
    _last_checked = limit;
}

/* Test primality of arbitrary candidate using _primes (which must be
 * generated up to at least sqrt(candidate) beforehand). */
static bool _is_prime(uint64_t n) {
    if (n < 2) return false;
    for (size_t i = 0; i < _prime_count; ++i) {
        uint64_t q = _primes[i];
        __uint128_t prod = (__uint128_t)q * (__uint128_t)q;
        if (prod > n) break;
        if (n % q == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------------- */
/*  Sophie Germain prime storage                                           */
/* --------------------------------------------------------------------------- */

static size_t _sg_cap = 0;
static size_t _sg_count = 0;
static uint64_t * _sg_primes = NULL;      /* list of SG primes */

static void _sg_append(uint64_t p) {
    if (_sg_count == _sg_cap) {
        size_t new_cap = (_sg_cap == 0) ? 32 : _sg_cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(_sg_primes, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        _sg_primes = tmp;
        _sg_cap = new_cap;
    }
    _sg_primes[_sg_count++] = p;
}

/* Generate SG primes until we have at least 'count' of them */
static void _ensure_sg_count(size_t count) {
    if (_sg_count >= count) return;

    uint64_t candidate = (_sg_count == 0) ? 2 : _sg_primes[_sg_count - 1] + 1;
    while (_sg_count < count) {
        if (candidate == 2) {
            /* 2 is prime, check 2p+1 = 5 */
            _generate_primes_up_to(5);
            if (_is_prime(2) && _is_prime(5)) {
                _sg_append(2);
            }
            candidate = 3;
        } else {
            if ((candidate & 1ULL) == 0) {
                ++candidate;
                continue;
            }
            /* generate enough primes to test candidate and 2c+1 */
            uint64_t q = 2 * candidate + 1;
            uint64_t max_needed = (q > candidate) ? q : candidate;
            _generate_primes_up_to(max_needed);

            if (_is_prime(candidate) && _is_prime(q)) {
                _sg_append(candidate);
            }
            candidate += 2;    /* only odd numbers need checking */
        }
    }
}

/* --------------------------------------------------------------------------- */
/*  Sequence function                                                         */
/* --------------------------------------------------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but safeguard */

    _ensure_sg_count((size_t)n);   /* lazily compute SG primes as needed */

    uint64_t p = _sg_primes[n - 1]; /* nth SG prime (1‑based) */
    /* compute 3*p + 1 in seq_t arithmetic */
    seq_t val = (seq_t)3 * (seq_t)p + (seq_t)1;
    return val;
}
