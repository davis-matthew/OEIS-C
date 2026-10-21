/*
 * A215309  Primes congruent to {1, 2, 3, 5} mod 19.
 * https://oeis.org/A215309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) are the primes congruent to 1,2,3,5 (mod 19)
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeTrial.h"

/* cached sequence values */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
/* next integer to test for primality */
static uint64_t current = 2;

/* ensure the cache has space for at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;           /* allocation failed – keep old cache */
    cache = tmp;
    cache_cap = new_cap;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset starts at 1; behaviour for 0 is irrelevant */

    /* Generate terms until the cache holds at least n elements */
    while (cache_len < (size_t)n) {
        /* Find next prime ≥ current */
        while (!isPrimeTrial((seq_t)current)) current++;

        /* Accept prime if its residue modulo 19 is 1,2,3,5 */
        uint32_t r = (uint32_t)(current % 19);
        if (r == 1 || r == 2 || r == 3 || r == 5) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)current;
        }
        current++;
    }

    return cache[n - 1];
}
