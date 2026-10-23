/*
 * A039432  Numbers whose base-10 representation has the same nonzero number
 *          of 6's and 7's.
 * https://oeis.org/A039432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   has_equal_nonzero_6_and_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for numbers whose decimal representation contains an equal,
   non‑zero, number of digits 6 and 7.  a(n) is the nth such number,
   with a(1)=67, a(2)=76, … */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_equal_nonzero_6_and_7.h"

/* Cache for the sequence terms.  cache[i] holds a(i+1).  We also
   remember the next integer to test so that the search continues
   where it left off, regardless of call order. */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t next_candidate = (seq_t)1;

/* Grow the cache array to hold at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    /* In the OEIS environment we assume memory allocation succeeds. */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Main sequence function.  The index is 1‑based. */
seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;   /* undefined for nonpositive indices */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        if (has_equal_nonzero_6_and_7(next_candidate)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}
