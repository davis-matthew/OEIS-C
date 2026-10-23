/*
 * A039081  Numbers whose base-9 representation has the same number of 0's and
 *          5's.
 * https://oeis.org/A039081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   equal_zeros_fives
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) is the n-th positive integer whose base‑9
   representation contains the same number of 0's and 5's. */

#include <stdlib.h>
#include "../utils/equal_zeros_fives.h"

static seq_t *cache      = NULL;   /* array of found numbers          */
static size_t cache_len  = 0;      /* current number of entries       */
static size_t cache_cap  = 0;      /* allocated capacity              */
static seq_t next_candidate = (seq_t)1;   /* next number to test          */

/* Append val to the cache, growing it if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Ensure that the cache contains at least 'need' elements. */
static void ensure_cache(idx_t need)
{
    while (cache_len < (size_t)need) {
        if (equal_zeros_fives(next_candidate))
            cache_append(next_candidate);
        ++next_candidate;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* undefined behaviour for offset 1, but safe */

    ensure_cache(n);
    return cache[n - 1];
}
