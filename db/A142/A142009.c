/*
 * A142009  Primes congruent to 5 mod 31.
 * https://oeis.org/A142009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 224, u32 n <= 147, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A142009: a(n) is the n‑th prime p with p ≡ 5 (mod 31). */
/* The function searches for primes congruent to 5 modulo 31, caches
   found terms and supports arbitrary access order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Cached list of term values */
static seq_t *cache = NULL;
static size_t cache_cap = 0; /* allocated capacity */
static size_t cache_len = 0; /* number of terms stored */

/* Next odd number to test.  Starts at the first candidate 5. */
static seq_t next_candidate = 5;

/* Ensure the cache has room for at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – the program cannot continue.  Abort. */
        abort();
    }
    cache     = tmp;
    cache_cap = new_cap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n=0 – return 0 */
        return 0;

    /* Generate terms until we have at least n terms stored. */
    while (cache_len < n) {
        /* Find the next prime congruent to 5 modulo 31. */
        while (true) {
            if ((next_candidate % 31) == 5 && is_prime_big(next_candidate))
                break;          /* prime found */
            next_candidate += 2; /* skip to next odd candidate */
        }

        /* Cache the found prime. */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = next_candidate;

        /* Advance to the next odd number after the prime. */
        next_candidate += 2;
    }

    return cache[n - 1]; /* a(1) is the first term */
}
