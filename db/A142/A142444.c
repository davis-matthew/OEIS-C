/*
 * A142444  Primes congruent to 36 mod 49.
 * https://oeis.org/A142444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 155, u32 n <= 109, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142444: primes congruent to 36 (mod 49) */

/* Uses a lazy, incremental primality test (is_prime_big) and a
 * dynamic cache to store found primes.  No sieve is needed.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cache of found primes ≡ 36 (mod 49).  */
static seq_t *cache      = NULL;   /* array of primes */
static size_t cache_len  = 0;      /* number of stored primes */
static size_t cache_cap  = 0;      /* current capacity of cache */

/* Next number of the form 49*k+36 to test for primality.  */
static idx_t next_candidate = 36;

/* Grow the cache if necessary to accommodate at least one more element. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache          = tmp;
    cache_cap      = new_cap;
}

/* Ensure that the cache holds at least n primes. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;

    /* Prepare cache if it's the first call. */
    if (!cache) grow_cache();

    while (cache_len < n) {
        /* Search for the next prime of the required form. */
        while (!is_prime_big((seq_t)next_candidate))
            next_candidate += 49;

        /* Store it. */
        if (cache_len + 1 > cache_cap) grow_cache();
        cache[cache_len++] = (seq_t)next_candidate;

        /* Prepare the candidate for the next search. */
        next_candidate += 49;
    }
}

/* OEIS sequence access function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset 0 is undefined */
    ensure_cache(n);
    return cache[n - 1];
}
