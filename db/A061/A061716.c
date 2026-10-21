/*
 * A061716  Binary order of n-th prime.
 * https://oeis.org/A061716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(log2(prime(n)))
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   _floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_floor_log2_uint64.h"

/* Internal state for on‑demand prime generation */

static uint64_t *_primes     = NULL;    /* 0‑based array of primes            */
static size_t  _prime_cnt   = 0;        /* number of primes stored           */
static size_t  _prime_cap   = 0;        /* allocated array capacity          */
static uint64_t _candidate   = 2;       /* next integer to test              */

/* Expand the prime array when it runs out of space */
static void _grow_prime_capacity(void)
{
    size_t new_cap = (_prime_cap == 0) ? 128 : _prime_cap * 2;
    uint64_t *tmp = (uint64_t *)realloc(_primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    _primes   = tmp;
    _prime_cap = new_cap;
}

/* Simple trial division using the primes already found */
static int _is_candidate_prime(uint64_t x)
{
    if (x < 2) return 0;
    for (size_t i = 0; i < _prime_cnt; ++i) {
        uint64_t p = _primes[i];
        if (p > x / p) break;          /* p*p > x */
        if (x % p == 0) return 0;      /* composite */
    }
    return 1;                          /* prime    */
}

/* Return the nth prime (1‑based index), generating as many primes as needed */
static uint64_t _nth_prime(idx_t n)
{
    while (_prime_cnt < (size_t)n) {
        if (_prime_cnt >= _prime_cap) _grow_prime_capacity();

        if (_is_candidate_prime(_candidate))
            _primes[_prime_cnt++] = _candidate;
        ++_candidate;
    }
    return _primes[n - 1];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for offset < 1 */

    uint64_t p   = _nth_prime(n);       /* nth prime (uint64_t)          */
    unsigned fl = _floor_log2_uint64(p);/* floor(log2(p))                */

    /* If p is a power of two, ceil equals fl; otherwise ceil = fl+1 */
    unsigned ceil = ((1ULL << fl) == p) ? fl : fl + 1;

    return (seq_t)ceil;                 /* cast to generic result type   */
}
