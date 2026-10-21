/*
 * A059269  Numbers m for which the number of divisors, tau(m), is divisible
 *          by 3.
 * https://oeis.org/A059269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _has_exp_mod3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the nth positive integer whose number of divisors
   is divisible by 3.  This holds iff the integer has a prime factor
   with exponent ≡ 2 (mod 3).  The function searches integers in
   increasing order and caches results so that repeated calls are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_has_exp_mod3.h"

/* cache for already-found terms */
static seq_t *cache   = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* last integer examined during caching (next candidate to test) */
static uint64_t next_candidate = 1;  /* the next value to consider */

/* ensure cache has room for at least n entries */
static void _ensure_cache_capacity(size_t n)
{
    if (n <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap < n)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);   /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined index, return 0 for safety */

    /* generate terms until we have the nth one */
    while (cache_len < (size_t)n) {
        /* go to the next integer to test */
        ++next_candidate;
        if (_has_exp_mod3(next_candidate)) {
            _ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
    }
    return cache[(size_t)n - 1];
}
