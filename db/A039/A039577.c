/*
 * A039577  Numbers whose base-6 representation has the same number of 0's,
 *          2's and 3's.
 * https://oeis.org/A039577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   has_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Search: generate numbers whose base‑6 representation has the same
 * number of 0's, 2's and 3's.  The sequence is returned in
 * non‑decreasing order; a(1) is the first such number (1), a(2)=4, etc.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_valid.h"

/* Cached results: a static array that grows as needed. */
static seq_t *cache     = NULL;   /* array of values */
static size_t cache_len   = 0;    /* number of values stored */
static size_t cache_cap   = 0;    /* allocated capacity */

/* Extend the cache to at least 'need' elements. */
static void extend_upto(idx_t need)
{
    if (cache_len >= need)
        return;

    /* Increase capacity if necessary. */
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)need)
            new_cap = (size_t)need;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Start searching from the next integer after the last cached value. */
    seq_t start = (cache_len == 0) ? 1 : cache[cache_len - 1] + 1;

    /* Find values until we have 'need' of them. */
    seq_t x = start;
    while (cache_len < (size_t)need) {
        if (has_valid(x))
            cache[cache_len++] = x;
        x++;
    }
}

/* Sequence function: returns a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    /* The offset of the sequence is 1. */
    if (n == 0)
        return 0;              /* undefined for n = 0; return 0 as safety */

    extend_upto(n);            /* ensure the cache contains at least n values */
    return cache[n - 1];       /* array is 0‑based; a(1) is at index 0 */
}
