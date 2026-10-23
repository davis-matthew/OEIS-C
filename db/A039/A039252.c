/*
 * A039252  Numbers whose base-12 representation has the same number of 4's
 *          and 10's.
 * https://oeis.org/A039252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   matches_base12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039252: numbers whose base‑12 representation has the same
 * count of digit 4 and digit 10 (digit 10 is 'A' in base‑12).  */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/matches_base12.h"

/* Return the n-th (1‑based) element of A039252.  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of stored terms */
    static idx_t cache_cap = 0;   /* allocated capacity   */
    static seq_t next_to_check = 0; /* candidate number */

    /* Ensure the cache can hold at least one more element. */
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                      /* allocation failure */
        cache = (seq_t *)tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        if (matches_base12(next_to_check)) {
            cache[cache_len++] = next_to_check;
            /* If we just filled the array, guarantee room for next run. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                void *tmp = realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return 0;
                cache = (seq_t *)tmp;
                cache_cap = (idx_t)new_cap;
            }
        }
        ++next_to_check;
    }

    return cache[n - 1];
}
