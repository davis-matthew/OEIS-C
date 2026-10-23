/*
 * A044210  Numbers k such that string 2,7 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_sub27
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches sequentially for numbers k such that
   the base‑8 representation of k contains the substring "27"
   but the representation of k‑1 does not.  The results are memoized
   in a static array so that repeated calls (in any order) are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_sub27.h"

/* Static cache of the first terms found. */
static seq_t *cache = NULL;          /* allocated array of seq_t */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* allocated capacity */

/* Grow the cache to hold at least min_cap elements. */
static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < min_cap) new_cap *= 2;
    seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_mem;
    cache_cap = new_cap;
}

/* Main entry: a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    /* Extend the cache if necessary */
    if (cache_len < n) {
        /* Start searching from the integer immediately after the last
           cached term, or from 1 if no terms have yet been cached. */
        seq_t k = (cache_len > 0) ? (cache[cache_len - 1] + 1) : 1;

        while (cache_len < n) {
            if (has_sub27(k) && !has_sub27(k - 1)) {
                grow_cache(cache_len + 1);
                cache[cache_len++] = k;
            }
            ++k;
        }
    }
    return cache[n - 1];
}
