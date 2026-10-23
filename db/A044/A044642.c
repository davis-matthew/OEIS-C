/*
 * A044642  Numbers k such that string 1,1 occurs in the base 9 representation
 *          of k but not of k+1.
 * https://oeis.org/A044642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has11_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth integer k such that the base‑9 representation of k
   contains the substring "11", but the base‑9 representation of k+1 does not.
   This is OEIS A044642. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has11_base9.h"

/* Cache of already found sequence values. */
static seq_t *cache        = NULL;
static size_t cache_len    = 0;   /* number of stored values */
static size_t cache_cap    = 0;   /* allocated capacity */
static idx_t last_scanned  = 0;   /* largest x already examined */

/* Grow the cache if necessary. */
static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Append a new value to the cache. */
static void cache_append(seq_t val)
{
    ensure_cache_capacity();
    cache[cache_len++] = val;
}

/* Expand the cache until it contains at least n values. */
static void ensure_cache_size(idx_t n)
{
    while (cache_len < (size_t)n) {
        if (has11_base9(last_scanned) && !has11_base9(last_scanned + 1))
            cache_append((seq_t)last_scanned);
        last_scanned++;
    }
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0; safe fallback */
    ensure_cache_size(n);
    return cache[n - 1];
}
