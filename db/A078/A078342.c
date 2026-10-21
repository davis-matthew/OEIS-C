/*
 * A078342  Number of positive integers less than n that are coprime to all
 *          primes less than or equal to the square root of n.
 * https://oeis.org/A078342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n-1) - pi(floor(sqrt(n))) + 1 (n > 1) , a(1) = 0
 * Range:     u8 n <= 1721, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---- Prime cache ----------------------------------------------------- */
static idx_t  _prime_cache_limit = 0;        /* maximum n for which primes are cached */
static size_t _prime_cache_cap   = 0;        /* number of primes stored   */
static uint64_t * _prime_cache    = NULL;     /* array of primes           */

/* Build or extend the prime cache up to at least ‘limit’.  For the
 * relatively small limits used in this sequence (≈ 10 000) the simple
 * sieve works fast enough and keeps the code portable to both C and C++. */
static void _ensure_primes(idx_t limit)
{
    if (limit <= _prime_cache_limit)
        return;                                 /* already available */

    size_t sz = (size_t)limit + 1;               /* array size for sieve */
    bool *sieve = (bool*)malloc(sz * sizeof(bool));
    if (!sieve)
        return;                                 /* out of memory – leave cache unchanged */

    for (size_t i = 0; i < sz; ++i)
        sieve[i] = true;
    if (sz > 0) sieve[0] = false;
    if (sz > 1) sieve[1] = false;

    for (idx_t p = 2; (idx_t)p * p <= limit; ++p) {
        if (!sieve[p])
            continue;
        for (idx_t q = p * p; q <= limit; q += p)
            sieve[q] = false;
    }

    /* Count primes to allocate the cache */
    size_t new_cap = 0;
    for (size_t i = 0; i < sz; ++i)
        if (sieve[i])
            ++new_cap;

    uint64_t *new_cache = (uint64_t*)malloc(new_cap * sizeof(uint64_t));
    if (!new_cache) {                     /* memory error – keep old cache */
        free(sieve);
        return;
    }

    size_t pos = 0;
    for (size_t i = 2; i < sz; ++i)
        if (sieve[i])
            new_cache[pos++] = (uint64_t)i;

    /* Replace the old cache */
    free(_prime_cache);
    _prime_cache = new_cache;
    _prime_cache_cap = new_cap;
    _prime_cache_limit = limit;

    free(sieve);
}

/* Return π(x): number of primes ≤ x. */
static seq_t _pi_value(idx_t x)
{
    if (x < 2)
        return 0;                           /* no primes < 2 */

    _ensure_primes(x);

    seq_t count = 0;
    for (size_t i = 0; i < _prime_cache_cap; ++i) {
        if (_prime_cache[i] <= x) {
            ++count;
        } else {
            break;                           /* primes are sorted */
        }
    }
    return count;
}

/* ---- Sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                           /* a(1) = 0 */

    idx_t n_minus_one = n - 1;
    idx_t sqrt_n = (idx_t)floor(sqrt((double)n));

    seq_t pi1 = _pi_value(n_minus_one);
    seq_t pi2 = _pi_value(sqrt_n);

    return pi1 - pi2 + 1;
}
