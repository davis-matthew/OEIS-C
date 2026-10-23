/*
 * A142607  Primes congruent to 8 mod 55.
 * https://oeis.org/A142607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 8 (mod 55)
 * Range:     u8 n <= 1, u16 n <= 165, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function keeps a growing static cache of primes that satisfy
 * the congruence and returns the n-th element of that cache.
 * The cache is filled lazily, so repeated calls for any n >= 1
 * are independent of call order.  All arithmetic uses the
 * integer types supplied by the build (idx_t, seq_t). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"   /* provides bool is_prime_small(seq_t) */

static idx_t   _last_checked   = 0; /**< last integer examined */
static seq_t * _prime_cache    = NULL; /**< cached primes ≡ 8 (mod 55) */
static idx_t   _prime_count    = 0; /**< number of cached primes */
static size_t  _prime_cap      = 0; /**< capacity of the cache */

/* Append a new prime to the cache, growing the array if necessary. */
static void _append_prime(seq_t prime)
{
    if (_prime_count >= _prime_cap) {
        /* grow capacity */
        _prime_cap = (_prime_cap == 0) ? 64 : (_prime_cap << 1);
        /* cast the result of realloc to the proper type so the code
           compiles in both C and C++ */
        _prime_cache = (seq_t *)realloc(_prime_cache, _prime_cap * sizeof(seq_t));
    }
    _prime_cache[_prime_count++] = prime;
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1; for n < 1 return a safe default */
    if (n < 1) return 0;

    /* Ensure the cache holds at least n primes */
    while (_prime_count < n) {
        /* Search for the next prime ≡ 8 (mod 55) */
        for (idx_t cand = _last_checked + 1; ; ++cand) {
            if ((cand % 55) == 8) {
                if (is_prime_small((seq_t) cand)) {
                    _append_prime((seq_t) cand);
                    _last_checked = cand;
                    break;   /* found one, continue outer while */
                }
            }
        }
    }

    return _prime_cache[n - 1];
}
