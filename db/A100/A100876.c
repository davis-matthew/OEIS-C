/*
 * A100876  Least number of squares that sum to prime(n).
 * https://oeis.org/A100876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least number of squares summing to the nth prime
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute the nth prime by incremental trial division and then
   decide the number of squares using the known congruence rules: 2 for p=2 or p≡1 (mod 4),
   3 for p≡3 (mod 8), and 4 otherwise. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Cached list of primes up to the largest n requested. */
static idx_t _prime_count = 0;      /* number of primes currently stored */
static idx_t _prime_cap = 0;        /* allocated capacity of the array */
static idx_t * _prime_cache = NULL; /* array of primes (uint64_t or larger) */
static idx_t _last_checked = 1;     /* last integer examined for primality */

/* Ensure the prime cache holds at least 'need' primes. */
static void _ensure_prime_cache(idx_t need)
{
    if (_prime_count >= need)
        return;

    /* Allocate or grow the cache array. */
    if (_prime_cap == 0) {
        _prime_cap = 64;
        _prime_cache = (idx_t *)malloc(_prime_cap * sizeof(idx_t));
        if (!_prime_cache) exit(1);
        /* Add the first prime, 2. */
        _prime_cache[_prime_count++] = 2;
        _last_checked = 2;
    } else if (_prime_cap - _prime_count < (need - _prime_count)) {
        _prime_cap <<= 1; /* double capacity */
        idx_t *newp = (idx_t *)realloc(_prime_cache, _prime_cap * sizeof(idx_t));
        if (!newp) exit(1);
        _prime_cache = newp;
    }

    /* Find further primes until we have 'need' of them. */
    idx_t candidate = _last_checked + 1;
    while (_prime_count < need) {
        bool is_prime = true;
        /* Test divisibility by all known primes up to sqrt(candidate). */
        for (idx_t i = 0; i < _prime_count; ++i) {
            idx_t p = _prime_cache[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            _prime_cache[_prime_count++] = candidate;
        }
        _last_checked = candidate;
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    /* n is 1-based index; sequence defined for n>=1. */
    if (n < 1) return 0; /* undefined, return 0 */

    _ensure_prime_cache(n);
    idx_t p = _prime_cache[n - 1];  /* nth prime */

    /* Determine the least number of squares summing to p. */
    if (p == 2)
        return (seq_t)2;

    if (p % 4 == 1)
        return (seq_t)2;
    else if (p % 8 == 3)
        return (seq_t)3;
    else
        return (seq_t)4;
}
