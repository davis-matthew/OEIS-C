/*
 * A163635  a(n) = 3*A022004(n) + 8.
 * https://oeis.org/A163635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * p_n + 8 where p_n is the nth prime in a prime triple
 *            (p, p+2, p+6)
 * Range:     u8 n <= 4, u16 n <= 92, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static idx_t _next_candidate = 5;            /* next odd number to test for a triple */
static idx_t _generated_count = 0;           /* how many terms are cached */
static seq_t *_cache = NULL;                 /* cached values of a(n) */
static size_t _cache_cap = 0;                /* capacity of _cache */

/* Grow the cache if needed. */
static void _grow_cache(void)
{
    _cache_cap = (_cache_cap == 0) ? 16 : _cache_cap * 2;
    _cache = (seq_t *)realloc(_cache, _cache_cap * sizeof(seq_t));
}

/* Ensure that values up to index n are computed and stored in _cache. */
static void _ensure_cache(idx_t n)
{
    if (_cache == NULL)
    {
        _cache_cap = 16;
        _cache = (seq_t *)malloc(_cache_cap * sizeof(seq_t));
        _next_candidate = 5;          /* start at the first possible prime triple */
        _generated_count = 0;
    }

    while (_generated_count < n)
    {
        /* Scan for the next prime triple. */
        for (;;)
        {
            idx_t p = _next_candidate;
            if (is_prime(p) && is_prime(p + 2) && is_prime(p + 6))
            {
                /* Found a triple: compute a(n) = 3p + 8. */
                seq_t val = 3 * (seq_t)p + 8;
                if (_generated_count >= _cache_cap)
                    _grow_cache();
                _cache[_generated_count++] = val;

                /* Move candidate past this prime to avoid repeats. */
                _next_candidate = p + 2;
                break;
            }
            _next_candidate += 2;          /* skip to next odd number */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; n==0 is undefined, return 0 */
        return 0;
    _ensure_cache(n);
    return _cache[n - 1];
}
