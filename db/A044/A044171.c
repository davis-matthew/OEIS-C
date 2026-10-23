/*
 * A044171  Numbers n such that string 4,5 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_45
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_45.h"

/* Sequence A044171: numbers whose base‑7 representation contains the
   substring “45” but the preceding integer does not.  This function
   computes the n‑th term (offset 1) by scanning the integers in order
   and memoising the results. */

/* Static cache for memoisation */
static idx_t *cache    = NULL;   /* array of found terms (idx_t is small enough) */
static size_t cache_len  = 0;    /* number of terms stored */
static size_t cache_cap  = 0;    /* current allocated capacity */
static idx_t  max_checked = 0;  /* largest integer examined so far */

/* Ensure the cache can hold at least n elements. */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n;
    if (cache_cap < required) {
        size_t new_cap = cache_cap == 0 ? 128 : cache_cap * 2;
        if (new_cap < required)
            new_cap = required;
        cache = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
        cache_cap = new_cap;
    }
}

/* Primary sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for offset 0 */
        return 0;

    if (cache_len >= n)        /* already known */
        return (seq_t)cache[n - 1];

    ensure_cache(n);

    /* Scan integers starting after the last examined value. */
    idx_t m = max_checked + 1;
    while (cache_len < n) {
        if (has_45(m) && !has_45(m - 1))
            cache[cache_len++] = m;
        m++;
    }
    max_checked = m - 1;
    return (seq_t)cache[n - 1];
}
