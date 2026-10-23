/*
 * A142814  Primes congruent to 16 mod 61.
 * https://oeis.org/A142814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 113, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142814: primes congruent to 16 modulo 61. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Static cache of found primes */
static seq_t *cache = NULL;      /* array of primes ≡ 16 (mod 61) */
static size_t cache_size = 0;    /* allocated size of cache array */
static size_t cache_used = 0;    /* number of primes stored in cache */

/* Next candidate number to test (always ≡ 16 (mod 61)) */
static uint64_t candidate = 16;  /* first number that satisfies the congruence */

/* Ensure the cache has at least 'min' primes */
static void grow_cache(size_t min)
{
    if (cache_size >= min)
        return;
    size_t newsize = cache_size ? cache_size * 2 : 256;
    while (newsize < min)
        newsize *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    cache = tmp;
    cache_size = newsize;
}

static void ensure_cache(size_t min)
{
    if (cache_used >= min)
        return;
    grow_cache(min);
    while (cache_used < min) {
        if (is_prime((idx_t)candidate))
            cache[cache_used++] = (seq_t)candidate;
        candidate += 61ULL;   /* keep the congruence 16 (mod 61) */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* Offset is 1, value for n < 1 is undefined */
    ensure_cache((size_t)n);
    return cache[n - 1];
}
