/*
 * A044026  Numbers k such that 4 and 5 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has45or54
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search for numbers k that contain the adjacent digits "45" or "54"
   in their decimal representation, but whose successor k+1 does not. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has45or54.h"

/* Return the n-th term of A044026 (offset 1). */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t cache_len   = 0;
    static size_t cache_cap   = 0;
    static seq_t last_checked = 0;   /* largest k tested so far */

    if (n == 0)            /* undefined for offset 1 – guard anyway */
        return 0;

    /* Build cache until we have at least n entries. */
    while (cache_len < n) {
        seq_t k = ++last_checked;   /* candidate number */

        if (has45or54(k) && !has45or54(k + 1)) {
            /* store k in cache */
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                cache     = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = k;
        }
    }

    return cache[n - 1];
}
