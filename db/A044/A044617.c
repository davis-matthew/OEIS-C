/*
 * A044617  Numbers n such that string 6,1 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has61_in_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence has no simple closed formula.
   The function searches numbers sequentially for the first n
   that contain the base‑8 substring "61" while the next number
   does not, caching the results for subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has61_in_base8.h"

/* Static cache of found terms. */
static seq_t *cache     = NULL;   /* array of found terms */
static size_t cache_len = 0;      /* number of terms stored      */
static size_t cache_cap = 0;      /* allocated capacity         */
static idx_t next_candidate = 1;  /* next integer to test       */

/* Ensure at least 'need' slots are available in the cache. */
static void ensure_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return the n-th term (n ≥ 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* undefined for n = 0, guard nevertheless */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t m = (seq_t)next_candidate;
        if (has61_in_base8(m) && !has61_in_base8(m + (seq_t)1)) {
            ensure_cap(cache_len + 1);
            cache[cache_len++] = m;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
