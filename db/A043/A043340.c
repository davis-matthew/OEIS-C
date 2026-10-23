/*
 * A043340  Numbers having four 1's in base 4.
 * https://oeis.org/A043340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer that has exactly four 1's in
 *            its base‑4 representation
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_ones_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_ones_base4.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n < offset: value irrelevant */

    /* Static cache for memoised terms. */
    static seq_t *cache   = NULL;  /* array of results */
    static idx_t  cache_cap = 0;   /* allocated capacity */
    static idx_t  cache_len = 0;   /* number of terms stored */
    static seq_t next_candidate = 1; /* next integer to test */

    /* Grow cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Find next integer that contains exactly four base‑4 1's. */
        while (count_ones_base4(next_candidate) != 4)
            ++next_candidate;

        /* Allocate more space if needed. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* allocation failure: return 0 */
            cache = tmp;
            cache_cap = (idx_t)new_cap;
        }

        /* Store the found term. */
        cache[cache_len++] = next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];          /* return the n‑th term (1‑based). */
}
