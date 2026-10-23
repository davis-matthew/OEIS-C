/*
 * A127589  Primes of the form 16k + 5.
 * https://oeis.org/A127589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 5 modulo 16 (primes of the
 *            form 16k + 5)
 * Range:     u8 n <= 8, u16 n <= 826, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* The function returns seq_t type which is a unsigned integer type
   defined by the build system.  The implementation below uses a
   simple incremental sieve:  all candidates of the form 16k+5 are
   checked for primality using the helper is_prime_small().  The
   results are cached so that any call to a() is O(1) after the
   necessary primes have been generated. */

static seq_t *cache = NULL;      /* array of found primes */
static size_t cache_len = 0;     /* number of primes stored */
static size_t cache_cap = 0;     /* capacity of cache array */
static uint64_t next_candidate = 5;  /* next 16k+5 number to test */

/* Ensure that the cache contains at least n primes.
   The cache is grown exponentially to keep amortised insert cost O(1). */
static void ensure_cache(idx_t n)
{
    while (cache_len < n)
    {
        if (cache == NULL)
        {
            cache_cap = 32;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (cache == NULL) abort();
        }
        else if (cache_len == cache_cap)
        {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = tmp;
        }

        if (is_prime_small(next_candidate))
            cache[cache_len++] = (seq_t)next_candidate;
        next_candidate += 16;   /* next candidate of the form 16k+5 */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1, value for n=0 is undefined */
    ensure_cache(n);
    return cache[n - 1];
}
