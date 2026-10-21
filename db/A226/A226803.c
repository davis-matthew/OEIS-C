/*
 * A226803  Primes p where the digital sum of p^2 is equal to 7.
 * https://oeis.org/A226803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, bref, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   digit_sum, next_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime p such that the digit sum of p² equals 7.
   Sequence A226803: p where sum_of_digits(p²) == 7. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/next_prime.h"
#include "../utils/seq_mul.h"
#include "../utils/digit_sum.h"

/* Cache of qualifying primes. */
static seq_t* _cache      = NULL;   /* array of primes */
static size_t _cache_len  = 0;      /* number of cached primes   */
static size_t _cache_cap  = 0;      /* current array capacity    */
static uint64_t _last_check = 1;    /* last number checked for a prime */

/* Ensure that at least n primes are cached. */
static void _ensure_cache(idx_t n)
{
    size_t target = (size_t)n;
    while (_cache_len < target) {
        /* Find next prime after the last checked number. */
        uint64_t p = next_prime(_last_check);
        _last_check = p;               /* update for next iteration */

        /* Compute p² and its decimal digit sum. */
        seq_t sq   = seq_mul((seq_t)p, (seq_t)p);
        seq_t sum  = digit_sum(sq);

        /* Store prime if the condition holds. */
        if (sum == (seq_t)7) {
            if (_cache_len == _cache_cap) {
                size_t new_cap = _cache_cap ? _cache_cap * 2 : 16;
                seq_t* tmp = (seq_t*)realloc(_cache, new_cap * sizeof(seq_t));
                _cache = tmp;
                _cache_cap = new_cap;
            }
            _cache[_cache_len++] = (seq_t)p;
        }
    }
}

/* External sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n < 1 */
    _ensure_cache(n);                  /* make sure nth term is ready */
    return _cache[(size_t)(n - 1)];    /* zero‑based indexing */
}
