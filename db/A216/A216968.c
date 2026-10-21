/*
 * A216968  Numbers k such that 2*k^2 + 3 is prime.
 * https://oeis.org/A216968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 33, u32 n <= 3507, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that 2*k^2 + 3 is prime. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

/* Static cache of the found k-values. */
static seq_t *cache = NULL;      /* pointer to allocated array */
static size_t cache_len = 0;     /* number of cached entries */
static size_t cache_cap = 0;     /* current capacity of the array */
static seq_t tested_k = 0;       /* next candidate k to test */

/* Ensure the cache has at least one unused slot. */
static void
ensure_cache_capacity(void)
{
    if (cache_len < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    void *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (tmp != NULL) {
        cache = (seq_t *)tmp;
        cache_cap = new_cap;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;           /* offset is 1; n=0 is undefined */

    /* Extend the cache until it contains at least n entries. */
    while (cache_len < (size_t)n) {
        /* Candidate k is tested_k. */
        seq_t k = tested_k;
        seq_t val = 2 * k * k + 3;   /* compute 2*k^2 + 3 */

        /* Check primality using the helper from utils. */
        if (isPrimeGeneric(val)) {
            ensure_cache_capacity();
            cache[cache_len++] = k;
        }
        ++tested_k;
    }
    return cache[(size_t)n - 1];
}
