/*
 * A201739  Numbers n such that 90*n + 29 is prime.
 * https://oeis.org/A201739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 681, u32 n <= 118, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integers k such that 90*k + 29 is prime; a(n) returns the n‑th such k. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cache of already found k values. */
static seq_t *cache = NULL;   /* cache[i] == a(i+1)  for i < terms_found */
static size_t cache_cap = 0;  /* allocated capacity of cache */
static idx_t terms_found = 0; /* number of terms stored in cache */
static idx_t next_k_to_check = 0; /* next k to examine in the search */

/* Ensure the cache is large enough for at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* memory allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n <= terms_found)
        return cache[n - 1];

    idx_t k = next_k_to_check;
    for (;;) {
        /* Compute 90*k + 29; cast k to seq_t to avoid intermediate overflow. */
        seq_t candidate = (seq_t)k * 90 + 29;
        if (is_prime_big(candidate)) {
            /* New term found: store k and update state. */
            ++terms_found;
            grow_cache(terms_found);
            cache[terms_found - 1] = (seq_t)k;

            if (terms_found == n) {
                next_k_to_check = k + 1;
                return cache[n - 1];
            }
        }
        ++k;
    }
}
