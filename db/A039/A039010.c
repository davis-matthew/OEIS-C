/*
 * A039010  Numbers whose base-5 representation has the same number of 1's and
 *          2's.
 * https://oeis.org/A039010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_ones_twos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑5 representation has the same number of 1's and 2's – 
   computed by brute‑force search with memoisation. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_equal_ones_twos.h"

/* Grow the cache so it can hold at least 'need' elements. */
static void ensure_cache_size(size_t need, seq_t **c, size_t *cap)
{
    if (*cap >= need) return;
    size_t new_cap = *cap ? (*cap << 1) : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*c, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);           /* OOM – not expected */
    *c = tmp;
    *cap = new_cap;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    /* Cache of previously found values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* The next integer to test. */
    static uint64_t next_num = 0;

    /* Expand the cache until we have at least n values. */
    while (cache_len < (size_t)n)
    {
        /* Find the next integer whose base‑5 representation has equal 1's and 2's. */
        while (!has_equal_ones_twos(next_num))
            ++next_num;

        /* Store the found value. */
        ensure_cache_size(cache_len + 1, &cache, &cache_cap);
        cache[cache_len] = (seq_t)next_num;
        ++cache_len;

        ++next_num;   /* Advance for the next search. */
    }

    return cache[(size_t)n - 1];
}
