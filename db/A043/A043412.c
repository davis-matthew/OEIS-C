/*
 * A043412  Numbers whose base-7 representation contains exactly four 4's.
 * https://oeis.org/A043412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   has_four_fours
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth integer whose base‑7 representation has exactly four 4’s.
 * It is the OEIS A043412: Numbers whose base‑7 representation contains
 * exactly four 4's.  The sequence starts 1600, 4001, 6402, …
 * We build the terms on demand, keeping a cache so later calls are fast.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_four_fours.h"

seq_t a(idx_t n)
{
    /* Cached terms, the current value of the next candidate, and cache size. */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;   /* Allocated capacity */
    static size_t  cache_sz  = 0;   /* Current number of terms stored */
    static seq_t  next_candidate = 1;   /* The next number to test */

    if (n == 0)
        return 0;          /* Out of bounds; not used per OEIS offset */

    /* Ensure the cache can hold all terms up to n */
    while (cache_sz < n) {
        /* Grow the cache if necessary */
        if (cache_cap == 0) {
            cache_cap = 128;
            cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
            if (!cache) return 0; /* Allocation failure – unlikely */
        } else if (cache_sz == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) return 0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Find the next number that satisfies the condition */
        while (!has_four_fours(next_candidate))
            ++next_candidate;

        /* Store it and advance the candidate for the next search */
        cache[cache_sz++] = next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
