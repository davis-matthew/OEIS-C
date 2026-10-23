/*
 * A023692  Numbers with a single 1 in their ternary expansion.
 * https://oeis.org/A023692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 5363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   one_one_ternary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers with a single 1 in their ternary expansion.
   Computed by scanning integers and retaining those with exactly
   one 1 digit in base‑3. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/one_one_ternary.h"

/* Static cache of computed sequence values. */
static seq_t *cache      = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;
static seq_t  next_candidate = 1;          /* next integer to test */

/* Ensure the cache holds at least n entries. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    /* Allocate or grow the cache as needed. */
    if (cache == NULL) {
        cache_cap = 64;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    else if (cache_len + 1 > cache_cap) {
        /* Double capacity until it fits the new item. */
        while (cache_len + 1 > cache_cap)
            cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Scan integers until we have enough values. */
    while ((size_t)n > cache_len) {
        if (one_one_ternary(next_candidate)) {
            if (cache_len + 1 > cache_cap) {   /* grow if needed */
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* n≥1 by specification, but guard anyway */

    ensure_cache(n);
    return cache[n - 1];
}
