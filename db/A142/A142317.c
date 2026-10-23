/*
 * A142317  Primes congruent to 11 mod 45.
 * https://oeis.org/A142317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p with p ≡ 11 (mod 45)
 * Range:     u8 n <= 3, u16 n <= 276, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset 1: a(1) = 11 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;     /* array of found primes */
static size_t cache_size = 0;   /* number of primes stored so far */
static size_t cache_cap = 0;    /* capacity of the cache array */
static seq_t next_candidate = 11; /* next number ≡ 11 (mod 45) to test */

static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_ptr == NULL) {
        /* In a full implementation you might handle the error,
           but for the purposes of this exercise we assume allocation succeeds. */
        exit(EXIT_FAILURE);
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* If no terms requested, return 0 (behavior below offset is irrelevant). */
    if (n == 0)
        return (seq_t)0;

    /* Generate terms until we have at least n terms. */
    while (cache_size < n) {
        /* Ensure we have a candidate that satisfies the congruence. */
        /* next_candidate already satisfies ≡ 11 (mod 45). */
        if (is_prime((idx_t)next_candidate)) {
            /* Append to cache */
            if (cache_size == cache_cap)
                grow_cache();
            cache[cache_size++] = next_candidate;
        }
        /* Move to next number that is 11 mod 45 */
        next_candidate += 45;
    }

    return cache[n - 1];
}
