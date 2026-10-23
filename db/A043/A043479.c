/*
 * A043479  Numbers having three 6's in base 9.
 * https://oeis.org/A043479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose base‑9 representation
 *            contains exactly three digits equal to 6
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   has_three_sixes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_three_sixes.h"

/* Dynamically sized cache for already found terms. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated capacity */
static size_t cache_len  = 0;   /* number of terms stored */
static seq_t  last_checked = (seq_t)0;   /* last integer examined */

/* Ensure the cache can hold at least cap entries. */
static void ensure_cache_cap(size_t cap)
{
    if (cap <= cache_cap) return;

    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap < cap) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) {
        abort(); /* allocation failure */
    }
    cache     = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence.  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n < 1 but harmless */

    while (cache_len < (size_t)n) {
        last_checked++;
        if (has_three_sixes(last_checked)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = last_checked;
        }
    }
    return cache[n - 1];
}
