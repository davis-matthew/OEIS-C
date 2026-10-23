/*
 * A139718  A number n is included if |k - n/k| divides n for at least one
 *          divisor k of n.
 * https://oeis.org/A139718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 526, u32 n <= 526, u64 n <= 526,
 *            u128 n <= 526, big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A139718: a(n) = the nth positive integer n that satisfies
   the property: there exists a divisor k of n such that |k – n/k| divides n */

#include <stdbool.h>
#include "../utils/grow_seq_array.h"

/* Test whether a given m has the required property. */
static bool has_property(idx_t m)
{
    seq_t mm = (seq_t)m;
    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d != 0)
            continue;
        idx_t md = m / d;                           /* paired divisor */
        seq_t diff = (seq_t)(d > md ? d - md : md - d);
        if (diff == 0)          /* avoid division by zero when m is a square */
            continue;
        seq_t rem = mm % diff;
        if (rem == 0)
            return true;
    }
    return false;
}

/* Return the nth term of the sequence (a(1) = 2, a(2) = 6, …).  */
seq_t a(idx_t n)
{
    /* Static cache of already found sequence elements. */
    static seq_t *cache      = NULL;   /* array of found terms          */
    static size_t cache_len  = 0;       /* current number of terms       */
    static size_t cache_cap  = 0;       /* allocated capacity of cache   */
    static idx_t  next        = 1;      /* next integer to test          */

    while (cache_len < (size_t)n) {
        if (has_property(next)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                /* grow_seq_array will reallocate cache and update cache_cap */
                grow_seq_array(&cache, &cache_cap, new_cap);
            }
            cache[cache_len++] = (seq_t)next;
        }
        ++next;
    }
    return cache[(size_t)n - 1];
}
