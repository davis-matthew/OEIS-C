/*
 * A046804  a(n) = p mod (p mod 10) where p = prime(n).
 * https://oeis.org/A046804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = p mod (p mod 10), where p = prime(n)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* We'll generate primes on demand by a simple incremental trial‑division
   algorithm.  The array size is chosen to safely hold all primes up to
   n = 10000 (prime(10000) = 104729). */
#define MAX_PRIMES   110000   /* a little more than 10000 primes */

static uint64_t _prime_cache[MAX_PRIMES];
static idx_t   _prime_count = 0;

/* Return true if x is prime by testing divisibility by all cached
   primes up to sqrt(x).  The array is guaranteed to hold all primes
   needed during this test because we fill it in order. */
static bool _is_prime(uint64_t x)
{
    for (idx_t i = 0; i < _prime_count; ++i) {
        uint64_t p = _prime_cache[i];
        if (p * p > x)
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/* Ensure that at least n primes are stored in _prime_cache.
   Generates missing primes by testing successive integers for primality. */
static void _ensure_primes(idx_t n)
{
    if (_prime_count >= n)
        return;

    if (_prime_count == 0) {
        _prime_cache[_prime_count++] = 2;
    }
    uint64_t candidate = _prime_cache[_prime_count - 1] + 1;
    while (_prime_count < n) {
        if (_is_prime(candidate)) {
            _prime_cache[_prime_count++] = candidate;
        }
        ++candidate;
    }
}

/* Sequence term function */
seq_t a(idx_t n)
{
    if (n == 0) /* not used, but keeps function robust */
        return 0;

    _ensure_primes(n);               /* make sure prime(n) is known */
    uint64_t p = _prime_cache[n - 1];/* prime index is 0‑based internally */

    seq_t P = (seq_t)p;             /* cast to seq_t for computation */
    seq_t q = P % 10;
    if (q == 0)                     /* never happens for primes, defensive */
        return 0;
    return P % q;
}
