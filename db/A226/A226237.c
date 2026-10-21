/*
 * A226237  Sum of the parts in the Goldbach partitions of 2n.
 * https://oeis.org/A226237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n * (#unordered Goldbach partitions of 2n)
 * Range:     u8 n <= 26, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t _sieve_limit = 0;
static uint32_t *_sieve_primes = NULL;
static size_t _sieve_prime_cnt = 0;
static uint8_t *_sieve_is_prime = NULL;

/* Build a sieve of Eratosthenes up to LIMIT (inclusive).  Rebuilds
   the sieve whenever a larger LIMIT is requested. */
static void _ensure_sieve(uint64_t limit)
{
    if (limit <= _sieve_limit) return;

    /* free previous tables */
    if (_sieve_is_prime) free(_sieve_is_prime);
    if (_sieve_primes) free(_sieve_primes);

    /* allocate and initialise prime‑indicator array */
    _sieve_is_prime = (uint8_t *)malloc((size_t)(limit + 1));
    if (_sieve_is_prime == NULL) abort();
    for (uint64_t i = 0; i <= limit; ++i) _sieve_is_prime[i] = 1;
    if (limit >= 0) _sieve_is_prime[0] = 0;
    if (limit >= 1) _sieve_is_prime[1] = 0;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (_sieve_is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                _sieve_is_prime[j] = 0;
        }
    }

    /* count primes and store them in an array */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (_sieve_is_prime[i]) ++cnt;

    _sieve_primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (_sieve_primes == NULL) abort();

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (_sieve_is_prime[i]) _sieve_primes[idx++] = (uint32_t)i;

    _sieve_prime_cnt = cnt;
    _sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* a(1)=0, and for n=0 also 0 */

    uint64_t two_n = 2 * (uint64_t)n;
    _ensure_sieve(two_n);

    seq_t count = 0;

    /* count unordered prime pairs (p, q) with p <= q, p + q = 2n */
    for (size_t i = 0; i < _sieve_prime_cnt && _sieve_primes[i] <= n; ++i) {
        uint32_t p = _sieve_primes[i];
        uint32_t q = (uint32_t)(two_n - p);
        if (q < p) continue;               /* ensure p <= q */
        if (_sieve_is_prime[q]) ++count;
    }

    return ((seq_t)two_n) * count;
}
