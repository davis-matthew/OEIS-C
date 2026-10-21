/*
 * A239211  Squares that are not divisible by any of their nonzero digits.
 * https://oeis.org/A239211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc, seq_realloc, square_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) from OEIS A239211.
   a(n) = the nth square whose decimal representation contains no
          non‑zero digit that divides the square itself.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/square_ok.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;                 /* out of range, may be ignored */

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of stored terms */
    static size_t cache_cap = 0;          /* allocated capacity */
    static idx_t last_m = 0;              /* largest m tested so far */

    /* Extend cache until we have at least n terms */
    while (cache_len < n) {
        ++last_m;
        if (square_ok(last_m)) {
            /* Ensure enough space */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();     /* memory exhausted */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_m * (seq_t)last_m;
        }
    }
    return cache[n - 1];
}
