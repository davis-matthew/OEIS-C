/*
 * A142923  Primes congruent to 61 mod 63.
 * https://oeis.org/A142923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 179, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 61 (mod 63) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Cache of primes found in the desired residue class. */
static seq_t *cache       = NULL;   /* array of primes */
static size_t cache_cap   = 0;      /* allocated capacity */
static size_t cache_len   = 0;      /* number of primes stored so far */
static idx_t  next_candidate = 61;  /* next number to test, always ≡61 (mod 63) */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    void *p = realloc(cache, new_cap * sizeof(seq_t));
    if (!p)
        abort(); /* unrecoverable failure */
    cache = (seq_t*)p;
    cache_cap  = new_cap;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    /* The sequence is 1‑based (offset 1). */
    while (cache_len < (size_t)n) {
        if (is_prime(next_candidate)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += 63; /* skip to the next number ≡61 (mod 63) */
    }
    return cache[n - 1];
}
