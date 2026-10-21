/*
 * A072284  Numbers k begins a new chain of squarefree integers. I.e., k is
 *          squarefree but k-1 is not.
 * https://oeis.org/A072284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   k is squarefree and k-1 is not squarefree (OEIS A072284)
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Upper bound for prime sieve – enough to test any candidate used up to
   the first 10 000 terms.  Compile‑time constant required for static
   arrays. */
#define PRIME_LIMIT 100000

/* Static storage for a small prime sieve */
static bool _prime_is[PRIME_LIMIT + 1];
static int  _prime_list[PRIME_LIMIT / 2 + 10];   /* enough for all primes <= 100000 */
static size_t _prime_cnt = 0;
static bool   _primes_built   = false;

/* Build the prime table once.  Linear sieve is easiest here. */
static void _build_primes(void)
{
    if (_primes_built) return;
    memset(_prime_is, true, sizeof(_prime_is));
    _prime_is[0] = _prime_is[1] = false;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (_prime_is[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                _prime_is[j] = false;
        }
    }
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (_prime_is[i]) _prime_list[_prime_cnt++] = i;
    _primes_built = true;
}

/* Test whether a 64‑bit unsigned integer is square‑free.
   Only primes up to sqrt(v) are tested.          */
static bool _is_squarefree(uint64_t v)
{
    if (v <= 1U) return true;
    for (size_t i = 0; i < _prime_cnt; ++i) {
        uint64_t p = (uint64_t)_prime_list[i];
        uint64_t sq = p * p;
        if (sq > v) break;
        if ((v % sq) == 0U) return false;
    }
    return true;
}

/* Cache to store terms that have already been discovered. */
static seq_t * _cache     = NULL;   /* array indexed 0…size-1, 0 unused */
static size_t _cache_size = 0;      /* highest index stored (initially 0) */
static size_t _cache_cap  = 0;
static uint64_t _next_candidate = 2;   /* next integer to test */

/* Ensure _cache can hold indices up to n (inclusive). */
static void _grow_cache(size_t n)
{
    if (_cache_cap > n) return;
    size_t new_cap = (_cache_cap == 0) ? 16 : _cache_cap * 2;
    while (new_cap <= n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(_cache, sizeof(seq_t) * new_cap);
    if (!tmp) abort();           /* abort on allocation failure */
    _cache = tmp;
    _cache_cap = new_cap;
}

/* Main sequence function.  Calls may arrive in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for the problem, safe value */

    /* Initialisation on first call */
    if (!_primes_built) _build_primes();
    if (_cache_size == 0) {
        _grow_cache(2);                /* at least enough for index 1 and possible 2 */
        _cache[0] = (seq_t)0;          /* unused, keeps indices 1‑based */
        _cache[1] = (seq_t)1;          /* a(1) = 1 */
        _cache_size = 1;               /* highest index stored */
    }

    /* Grow the cache until it contains at least the requested term. */
    while (_cache_size < (size_t)n) {
        uint64_t cand = _next_candidate;
        /* check the definition: square‑free and (cand‑1) not square‑free */
        if (_is_squarefree(cand) && !_is_squarefree(cand - 1U)) {
            ++_cache_size;             /* new slot for the term */
            _grow_cache(_cache_size);
            _cache[_cache_size] = (seq_t)cand;
        }
        ++_next_candidate;
    }

    return _cache[(size_t)n];
}
