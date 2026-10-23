/*
 * A039047  Numbers whose base-7 representation has the same number of 4's and
 *          5's.
 * https://oeis.org/A039047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th non‑negative integer whose base‑7 representation
 *            contains the same number of digit 4’s as digit 5’s
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_4_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_4_5.h"

/* Static state to cache computed values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t candidate = 0;

/* Ensure the cache can hold at least 'need' elements.  Capacity is
   doubled each time for amortized linear growth. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* a(n) as defined in the OEIS.  n >= 1, but n == 0 returns 0. */
seq_t a(idx_t n)
{
    if (n == 0u)
        return 0u;

    /* Generate values until we have at least n terms cached. */
    while (cache_len < n) {
        if (cache_len >= cache_cap)
            ensure_cache_cap(cache_len + 1);

        if (has_equal_4_5(candidate))
            cache[cache_len++] = candidate;

        ++candidate;
    }

    return cache[n - 1];
}
