/*
 * A133850  Number of partitions of n into two primes and a semiprime.
 * https://oeis.org/A133850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered partitions of n into two primes * and
 *            one semiprime (a semiprime is a product of exactly two primes, *
 *            primes may be equal). OEIS offset 1. For example: * a(8) = 1 (2
 *            + 2 + 4) * a(9) = 1 (2 + 3 + 4) * a(10) = 2 (2 + 2 + 6, 3 + 3 +
 *            4)
 * Range:     u8 n <= 206, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static state reused across calls */
static struct {
    idx_t      limit;   /* maximum n processed so far */
    size_t     pcount;  /* number of primes up to limit */
    uint32_t  *primes;  /* array of primes */
    bool      *is_prime;   /* prime sieve flags */
    bool      *is_semiprime;/* semiprime flags */
} _state = {0,0,NULL,NULL,NULL};

/* Ensure that the sieve and prime list are built at least up to 'limit' */
static void _ensure_state(idx_t limit)
{
    if (limit <= _state.limit) return;

    /* free existing data */
    free((void*)_state.is_prime);
    free((void*)_state.is_semiprime);
    free((void*)_state.primes);

    /* allocate new arrays */
    _state.is_prime = (bool*)malloc((size_t)(limit + 1) * sizeof(bool));
    _state.is_semiprime = (bool*)malloc((size_t)(limit + 1) * sizeof(bool));
    _state.primes = (uint32_t*)malloc((size_t)_state.pcount * sizeof(uint32_t));
    if (!_state.is_prime || !_state.is_semiprime || !_state.primes)
        abort();

    /* sieve of Eratosthenes */
    for (idx_t i = 0; i <= limit; ++i) _state.is_prime[i] = true;
    if (limit >= 0) _state.is_prime[0] = false;
    if (limit >= 1) _state.is_prime[1] = false;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (_state.is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                _state.is_prime[j] = false;
        }
    }

    /* collect primes into array */
    _state.pcount = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (_state.is_prime[i]) _state.pcount++;
    /* reallocate primes array with correct size */
    free((void*)_state.primes);
    _state.primes = (uint32_t*)malloc(_state.pcount * sizeof(uint32_t));
    if (!_state.primes) abort();

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (_state.is_prime[i]) _state.primes[pos++] = (uint32_t)i;

    /* compute semiprime flags */
    for (idx_t i = 0; i <= limit; ++i) _state.is_semiprime[i] = false;
    for (size_t a = 0; a < _state.pcount; ++a) {
        uint32_t p = _state.primes[a];
        for (size_t b = a; b < _state.pcount; ++b) {
            uint32_t q = _state.primes[b];
            uint64_t prod = (uint64_t)p * (uint64_t)q;
            if (prod > limit) break;
            _state.is_semiprime[(idx_t)prod] = true;
        }
    }

    _state.limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 8)   /* all terms below 8 are 0, no need to compute */
        return 0;

    _ensure_state(n);

    seq_t result = 0;
    for (size_t i = 0; i < _state.pcount; ++i) {
        uint32_t p = _state.primes[i];
        if ((idx_t)p > n) break;
        for (size_t j = i; j < _state.pcount; ++j) {
            uint32_t q = _state.primes[j];
            idx_t sum = (idx_t)p + (idx_t)q;
            if (sum > n) break;
            idx_t r = n - sum;
            if (r > 0 && r <= _state.limit && _state.is_semiprime[r])
                result += (seq_t)1;
        }
    }
    return result;
}
