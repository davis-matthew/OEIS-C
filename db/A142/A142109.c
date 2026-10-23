/*
 * A142109  Primes congruent to 29 mod 36.
 * https://oeis.org/A142109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 538, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 29 modulo 36.  The code generates
   candidates of the form 29+36*k and keeps those that are prime.
   Cache is maintained so that each prime is computed only once. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Cached primes ≡ 29 (mod 36) – element 0 corresponds to a(1). */
static seq_t *cache = NULL;          /* array of cached primes      */
static size_t cache_cap = 0;         /* capacity of the array       */
static size_t cache_len = 0;         /* current number of cached primes */
static idx_t next_step = 0;          /* next k to try for candidate 29+36*k */

/* ensure that cache has at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* unlikely, but safe */
    cache = tmp;
    cache_cap = new_cap;
}

/* make sure that we have at least n primes ≡ 29 mod 36 in the cache */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    while (cache_len < (size_t)n) {
        idx_t candidate = 29 + next_step * 36;
        if (is_prime(candidate)) {
            if (cache_len == cache_cap)
                grow_cache(cache_cap ? cache_cap << 1 : 16);
            cache[cache_len++] = (seq_t)candidate;
        }
        ++next_step;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for offset 0 */
    ensure_cache(n);
    return cache[n - 1];
}
