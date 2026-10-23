/*
 * A043925  Numbers k such that 2 and 6 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_adj26
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers k whose base‑7 representation contains the adjacent
   pair “26” or “62” but k+1 does not.  a(1) = 20. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adj26.h"

/* Return the n-th term of the sequence (offset 1).  Calls are
   memoized so that any order of n is supported. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* behavior below the offset is irrelevant */

    /* ---------- cache of already found terms ---------- */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 0;   /* largest integer examined so far */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++last_checked;                     /* next candidate k */

        seq_t next = last_checked + 1;
        if (has_adj26(&last_checked) && !has_adj26(&next)) {
            /* grow cache if needed */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                if (new_cap == 0) new_cap = 16;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_checked;
        }
    }

    return cache[(size_t)(n - 1)];
}
