/*
 * A045274  Numbers whose base-5 representation contains exactly three 2's and
 *          no 3's.
 * https://oeis.org/A045274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth number whose base-5 representation contains exactly
 *            three 2's and no 3's (OEIS A045274)
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   valid_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/valid_uint64.h"

/* Static cache for memoisation. */
static seq_t *cache = NULL;     /* array of already found terms */
static size_t cache_len = 0;    /* number of terms stored */
static size_t cache_cap = 0;    /* allocated capacity */
static uint64_t next_candidate = 0;  /* next integer to test */

/* Expand cache to at least new_cap elements. */
static void ensure_cache_cap(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t grow = cache_cap > 0 ? cache_cap : 8;
    while (grow < new_cap) grow <<= 1;
    cache = (seq_t *)realloc(cache, grow * sizeof(seq_t));
    cache_cap = grow;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for offset 1, but keep safe */

    /* Build cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* Find next valid number. */
        while (!valid_uint64(next_candidate))
            ++next_candidate;          /* skip invalid numbers */

        /* Store the valid number. */
        ensure_cache_cap(cache_len + 1);
        cache[cache_len] = (seq_t)next_candidate;
        ++cache_len;
        ++next_candidate;              /* advance to start of next search */
    }

    return cache[(size_t)n - 1];        /* cache is 0‑based, a(1) is first */
}
