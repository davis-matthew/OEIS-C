/*
 * A043468  Numbers having four 3's in base 9.
 * https://oeis.org/A043468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   hasFour3s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers having four 3's in base 9.  */
/*  This implementation iteratively tests every integer starting from 1
 *  and counts those whose base‑9 representation contains exactly four
 *  digits equal to 3.  Generated terms are cached to allow repeated
 *  calls to a(idx_t n) in any order.  */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/hasFour3s.h"

static seq_t *cache = NULL;      /* cached terms */
static size_t cache_len = 0;     /* number of cached terms */
static size_t cache_cap = 0;     /* allocated capacity */
static seq_t next_candidate = 1; /* next integer to test */

/*  Ensure the cache has space for at least new_cap elements.  */
static void grow_cache(size_t new_cap)
{
    /*  Reallocate the array to the new capacity.  */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /*  Allocation failed – abort: sequence cannot be computed.  */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/*  Return the n-th term of the sequence (1‑based).  */
seq_t a(idx_t n)
{
    /*  Generate terms until we have at least n of them.  */
    while (cache_len < (size_t)n) {
        /*  Expand the cache if necessary.  */
        if (cache == NULL) {
            grow_cache(1024);
        } else if (cache_len == cache_cap) {
            grow_cache(cache_cap + 1024);
        }

        /*  Test the current candidate.  */
        if (hasFour3s(next_candidate)) {
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}
