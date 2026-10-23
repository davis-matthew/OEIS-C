/*
 * A130775  a(1) = 0; for n > 1: a(n) = 2*(prime(n)-1)!/(prime(n)+1).
 * https://oeis.org/A130775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 0; for n > 1: a(n) = 2*(prime(n)-1)!/(prime(n)+1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --------------------------------------------------------------------------
   Prime generation (trial division) – works for n up to about 10 000
   -------------------------------------------------------------------------- */
#define MAX_PRIMES 11000   /* enough for the first 10 000 primes */

static uint32_t _primes[MAX_PRIMES];
static size_t   _prime_cnt = 0;
static uint32_t _candidate  = 2; /* last examined integer */

/* Push the next prime into the prime table. */
static void _next_prime(void)
{
    for (;;)
    {
        /* Skip even numbers after 2. */
        _candidate += (_candidate == 2) ? 1 : 2;
        bool is_prime = true;
        for (size_t i = 0; i < _prime_cnt; ++i)
        {
            uint32_t p = _primes[i];
            uint64_t sq = (uint64_t)p * (uint64_t)p;
            if (sq > _candidate) break;
            if (_candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime)
        {
            if (_prime_cnt < MAX_PRIMES)
                _primes[_prime_cnt++] = (uint32_t)_candidate;
            return;
        }
    }
}

/* Return the k‑th prime, building the table if necessary. */
static uint32_t _nth_prime(idx_t k)
{
    if (_prime_cnt == 0)
    {
        _primes[_prime_cnt++] = 2;
        _candidate = 2;
    }
    while (_prime_cnt < (size_t)k) _next_prime();
    return _primes[k - 1];
}

/* --------------------------------------------------------------------------
   Incremental factorial cache
   'fact_cache' holds (max_fact_prime_minus1)!   where
   max_fact_prime_minus1 = largest prime used in a(n) minus 1
   -------------------------------------------------------------------------- */
static seq_t   _fact_cache               = 1;
static uint32_t _fact_max_prime_minus_one = 0;

/* --------------------------------------------------------------------------
   Main sequence function
   -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    /* Largest prime required for this term */
    uint32_t p = _nth_prime((idx_t)n);

    /* Extend the cached factorial if necessary */
    if (_fact_max_prime_minus_one < p - 1)
    {
        uint32_t start = _fact_max_prime_minus_one + 1;
        for (uint32_t i = start; i <= p - 1; ++i)
            _fact_cache *= (seq_t)i;
        _fact_max_prime_minus_one = p - 1;
    }

    seq_t result = _fact_cache * (seq_t)2;
    result /= (seq_t)(p + 1);
    return result;
}
