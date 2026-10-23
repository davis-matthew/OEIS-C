/*
 * A039089  Numbers whose base-9 representation has the same number of 1's and
 *          7's.
 * https://oeis.org/A039089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   has_equal_ones_sevens
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sequence A039089: numbers whose base‑9 representation has the same
   number of 1s and 7s.  The implementation enumerates integers in
   ascending order, caching the results so that repeated calls are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_ones_sevens.h"

/* Cached terms and state. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t next_candidate = 0;   /* next integer to test */

/* initialise cache with the first term a(1)=0 */
static void init_cache(void)
{
    if (cache_len != 0)
        return;
    cache_cap = 64;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache)
        exit(1);
    cache[0] = (seq_t)0;          /* a(1) = 0 */
    cache_len = 1;
    next_candidate = (seq_t)1;    /* start searching from 1 */
}

/* ensure the cache can hold at least “needed” terms */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of A039089, with n >= 1. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;          /* undefined for n < 1 */

    init_cache();

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Find the next qualifying number. */
        while (!has_equal_ones_sevens(next_candidate))
            ++next_candidate;
        /* Store it. */
        grow_cache(cache_len + 1);
        cache[cache_len] = next_candidate;
        ++cache_len;
        /* Prepare for the next search. */
        ++next_candidate;
    }

    return cache[n - 1];
}
