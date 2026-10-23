/*
 * A045713  Primes with first digit 7.
 * https://oeis.org/A045713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th prime whose decimal representation starts with the
 *            digit 7
 * Range:     u8 n <= 4, u16 n <= 125, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

/* Cached sequence values */
static seq_t *cache = NULL;      /* dynamic array of found terms */
static size_t cache_cap = 0;     /* current capacity of cache */
static idx_t cache_count = 0;    /* number of terms stored (>=1) */
static uint64_t last_prime_checked = 2; /* last prime inspected */

/* Grow the cache if needed */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) {
        /* Failure is not recoverable; abort. */
        abort();
    }
    cache = new_buf;
    cache_cap = new_cap;
}

/* Return the nth term of A045713. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* Undefined for n < 1, return 0. */

    /* If already computed, return directly. */
    if (n <= cache_count)
        return cache[n - 1];

    /* Generate primes until we have the nth term. */
    while (cache_count < n) {
        /* Find the next prime after the last one we checked. */
        last_prime_checked = next_prime(last_prime_checked);
        /* Check if its first digit is 7. */
        seq_t x = (seq_t)last_prime_checked;
        while (x >= 10)
            x /= 10;
        if (x == 7) {
            /* Store this term in the cache. */
            ensure_cache_capacity(cache_count + 1);
            cache[cache_count] = (seq_t)last_prime_checked;
            cache_count++;
        }
    }

    return cache[n - 1];
}
