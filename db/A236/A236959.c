/*
 * A236959  Number of nonzero cubic residues modulo the n-th prime.
 * https://oeis.org/A236959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-1)/gcd(prime(n)-1,3) * that is, a(n) =
 *            (prime(n)-1)/3 if prime(n)≡1 (mod 3), * else a(n) = prime(n)-1
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   _sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_sieve_primes.h"

/* Cached list of primes.  On first call we generate at least the
 * n‑th prime; on later calls we extend the sieve only when needed. */
static uint64_t *_cached_primes = NULL;   /* array of primes */
static size_t   _cached_cnt    = 0;        /* number of cached primes  */
static uint64_t _cached_limit  = 0;        /* sieve limit for cached array */

/* Ensure that we have at least the n‑th prime cached.  The sieve limit
 * is chosen conservatively: for n<6 we only need primes up to 15,
 * otherwise we use a factor of 20 so that for n=10000 the limit is
 * 200 100 which comfortably exceeds 10 000‑th prime. */
static void _ensure_primes(idx_t n)
{
    uint64_t need_limit;
    if (n < 6) {
        need_limit = 15;
    } else {
        need_limit = (uint64_t)n * 20 + 100;
    }
    if (need_limit <= _cached_limit) return;

    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;
    _sieve_primes(need_limit, &new_primes, &new_cnt);

    free(_cached_primes);          /* free old cache if any */
    _cached_primes = new_primes;
    _cached_cnt    = new_cnt;
    _cached_limit  = need_limit;
}

/* Sequence entry: number of nonzero cubic residues modulo the n‑th prime. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* behaviour for n=0 is undefined */
    _ensure_primes(n);
    /* At this point _cached_cnt ≥ n */
    uint64_t p = _cached_primes[n - 1]; /* nth prime, 1‑based */
    seq_t result;
    if (p % 3 == 1) {
        result = (seq_t)((p - 1) / 3);
    } else {
        result = (seq_t)(p - 1);
    }
    return result;
}
