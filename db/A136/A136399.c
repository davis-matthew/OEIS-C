/*
 * A136399  Numbers having in their decimal representation at least one digit
 *          greater than 1.
 * https://oeis.org/A136399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 248, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_le1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n-th positive integer whose decimal representation 
   contains at least one digit greater than one (complement of numbers
   whose digits are only 0 or 1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digits_le1.h"

/* Lazy cache of precomputed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Grow the cache to accommodate at least 'need' elements */
static void enlarge_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);   /* abort on allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, return 0 for safety */
        return 0;

    enlarge_cache(n);

    /* Compute terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t last = (cache_len == 0) ? 0 : cache[cache_len - 1];
        seq_t cand = last + 1;
        /* Search for the next number that has a digit > 1 */
        while (digits_le1(cand)) {
            cand += 1;
        }
        cache[cache_len++] = cand;
    }

    return cache[n - 1];
}
