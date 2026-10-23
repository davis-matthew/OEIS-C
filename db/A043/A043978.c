/*
 * A043978  Numbers k such that 2 and 6 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_26
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043978: numbers k such that in base‑9 the digits 2 and 6
 * appear adjacent (in any order) in the representation of k but not in
 * k+1.  The function builds a running cache so that any call with
 * arbitrary order of n is answered in O(1) after the cache grows to size n. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_26.h"

/* cache of discovered terms */
static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */
static seq_t next_candidate = 1; /* smallest candidate to test */

/* ensure that the cache has capacity for at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* sequence is 1‑based; n=0 is unused */

    /* grow the cache until it contains at least n elements */
    while (cache_len < (size_t)n) {
        seq_t k = next_candidate;
        for (;;) {                /* find next valid k */
            if (has_26(k) && !has_26(k + 1)) {
                grow_cache(cache_len + 1);
                cache[cache_len++] = k;
                next_candidate = k + 1;
                break;
            }
            ++k;
        }
    }
    return cache[(size_t)n - 1];
}
