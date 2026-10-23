/*
 * A136208  Primes p such that p-8 or p+8 is prime.
 * https://oeis.org/A136208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p-8 or p+8 is prime (OEIS A136208) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

/* Cached terms: cache[i] holds a(i+1). */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static size_t cached_count = 0;
/* Next odd number to examine; starts at the first candidate prime. */
static idx_t last_checked = 3;

/* Ensure the cache can hold at least new_cap entries. */
static void ensure_cache_capacity(size_t new_cap)
{
    if (cache_capacity < new_cap)
    {
        size_t grow = (new_cap > 128) ? new_cap : 128;
        seq_t *tmp = (seq_t *)realloc(cache, grow * sizeof(seq_t));
        if (tmp == NULL) abort();        /* allocation failure */
        cache = tmp;
        cache_capacity = grow;
    }
}

/* Return the nth term (1‑based).  If n == 0 a dummy 0 is returned. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for non‑positive index */

    /* Extend the cache until it contains the requested term. */
    while (cached_count < (size_t)n)
    {
        /* Skip non‑primes. */
        while (!is_prime(last_checked)) last_checked += 2;

        /* Check the ±8 condition. */
        bool has_pair = false;
        /* p+8 prime ? (avoid overflow when adding 8 – not needed for
           the expected range, but kept for safety). */
        if (last_checked <= (idx_t)-8 && is_prime(last_checked + 8))
            has_pair = true;
        /* p-8 prime ? */
        if (!has_pair && last_checked >= 8 && is_prime(last_checked - 8))
            has_pair = true;

        if (has_pair)
        {
            ensure_cache_capacity(cached_count + 1);
            cache[cached_count] = (seq_t)last_checked;
            ++cached_count;
        }
        /* Move to the next odd candidate. */
        last_checked += 2;
    }

    return cache[n - 1];
}
