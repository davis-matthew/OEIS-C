/*
 * A039009  Numbers whose base-5 representation has the same number of 0's and
 *          3's.
 * https://oeis.org/A039009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_zeros_and_threes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039009: Numbers whose base‑5 representation has an equal
 * number of 0's and 3's.  The function generates terms by scanning
 * successive integers and keeping those that satisfy the property.  A
 * static cache is used so that repeated calls (in any order) are fast. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_equal_zeros_and_threes.h"

static seq_t *cache = NULL;      /* cached terms */
static idx_t  cache_len = 0;     /* number of cached terms */
static idx_t  cache_cap = 0;     /* allocated capacity */
static seq_t  last_checked = 0;  /* last number tested */

/* append a new term to the cache */
static void
push_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* generate terms until we have at least n terms cached */
static void
ensure_cache(idx_t n)
{
    if (cache_len >= n) return;

    seq_t cand = last_checked + (seq_t)1;
    while (cache_len < n) {
        if (has_equal_zeros_and_threes(cand)) {
            push_cache(cand);
        }
        cand += (seq_t)1;
    }
    last_checked = cand - (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* undefined for n=0, return 0 */
    ensure_cache(n);
    return cache[n-1];
}
