/*
 * A039543  Numbers whose base-12 representation has the same nonzero number
 *          of 5's and 11's.
 * https://oeis.org/A039543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   eq5_and_11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-12 representation has the same nonzero number of 5's and 11's. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/eq5_and_11.h"

/* Cache of found terms; grows as needed. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for offset < 1 */

    /* Expand cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        seq_t next = (cache_len == 0) ? 1 : cache[cache_len - 1] + (seq_t)1;

        /* Find the next valid number */
        while (!eq5_and_11(next))
            next++;

        /* Append to cache */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure, return 0 as safe fallback */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = next;
    }

    return cache[n - 1];
}
