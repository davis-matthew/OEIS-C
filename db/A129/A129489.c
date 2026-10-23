/*
 * A129489  Least k>1 such that binomial(2k,k) is not divisible by any of the
 *          first n odd primes.
 * https://oeis.org/A129489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  bref, hard, more, nonn
 * Formula:   a(n) = least k>1 such that binomial(2k,k) is not divisible * by
 *            any of the first n odd primes (3,5,7,...)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   _fits_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/_fits_prime.h"

/* Pre‑computed primes up to 200000 ensure the first 10000 odd primes
 * are available (the 10000th odd prime is 104729). */
#define MAX_SIEVE 200000
#define MAX_PRIMES 20000

static bool _sieve_done = false;
static uint32_t _primes[MAX_PRIMES];
static size_t _prime_count = 0;
static uint8_t _sieve[MAX_SIEVE + 1];

/* Generate primes once, using the sieve of Eratosthenes. */
static void _build_primes(void)
{
    if (_sieve_done) return;
    for (uint32_t i = 0; i <= MAX_SIEVE; ++i) _sieve[i] = 1;
    _sieve[0] = _sieve[1] = 0;
    for (uint32_t p = 2; p * p <= MAX_SIEVE; ++p) {
        if (_sieve[p]) {
            for (uint32_t m = p * p; m <= MAX_SIEVE; m += p)
                _sieve[m] = 0;
        }
    }
    /* Collect odd primes (2 is excluded) */
    size_t idx = 0;
    for (uint32_t num = 3; num <= MAX_SIEVE; ++num) {
        if (_sieve[num]) {
            _primes[idx++] = num;
            if (idx >= MAX_PRIMES) break;
        }
    }
    _prime_count = idx;
    _sieve_done = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1, so a(0) is undefined but return 0 */
    _build_primes();
    if (n > _prime_count) {
        /* For safety, if requested n exceeds our precomputed amount,
           we could extend the sieve, but the problem guarantees n ≤ 10000. */
        return (seq_t)0;
    }

    seq_t k = (seq_t)2; /* start searching from 2 (strictly >1) */
    for (;;) {
        /* Check all first n odd primes */
        bool ok = true;
        for (size_t i = 0; i < (size_t)n; ++i) {
            if (!_fits_prime(k, _primes[i])) {
                ok = false;
                break;
            }
        }
        if (ok) return k;
        ++k;
    }
}
