/*
 * A039484  Numbers whose base-11 representation has the same nonzero number
 *          of 6's and 8's.
 * https://oeis.org/A039484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A039484:
   Numbers whose base‑11 representation has the same nonzero number of 6's and 8's.
   Implementation: incremental brute‑force search with caching of found terms. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* cached terms of the sequence (1‑based indexing) */
static seq_t *cache = NULL;   /* pointer to the array of terms */
static size_t cache_len = 0;  /* number of terms stored in cache   */
static size_t cache_cap = 0;  /* allocated capacity of cache       */

/* next candidate integer to test when searching for more terms */
static seq_t next_candidate = 1;

/* Test whether a number has equal non‑zero counts of base‑11 digits 6 and 8. */
static bool qualifies(seq_t v)
{
    seq_t t = v;
    int c6 = 0, c8 = 0;
    while (t > 0) {
        int d = (int)(t % 11);      /* extract least significant base‑11 digit */
        if (d == 6) ++c6;
        else if (d == 8) ++c8;
        t /= 11;
    }
    return (c6 == c8) && (c6 > 0);
}

/* Grow the cache array to at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    seq_t *new_arr = alloc_seq_array(new_cap);
    if (!new_arr) {
        /* allocation failure – in this environment we simply abort */
        abort();
    }
    /* copy existing terms */
    for (size_t i = 0; i < cache_len; ++i) {
        new_arr[i] = cache[i];
    }
    /* free old array */
    if (cache) free_seq_array(cache);
    cache = new_arr;
    cache_cap = new_cap;
}

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for n = 0 */
    size_t target = (size_t)n;                 /* number of terms needed */

    /* If we already have enough terms, return the cached one. */
    if (cache_len >= target) {
        return cache[target - 1];
    }

    /* Ensure cache can hold the new terms. */
    if (cache_cap < target) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < target) new_cap = target;
        grow_cache(new_cap);
    }

    /* Scan natural numbers until we collect enough terms. */
    while (cache_len < target) {
        if (qualifies(next_candidate)) {
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[target - 1];
}
