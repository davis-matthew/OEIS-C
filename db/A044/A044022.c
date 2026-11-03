/*
 * A044022  Numbers k such that 3 and 6 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching for all numbers k that contain
   the substring "36" or "63" in base‑10, but k+1 does not. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: does decimal representation of x contain "36" or "63"? */
static inline bool contains_36_or_63(uint64_t x)
{
    while (x >= 10ULL) {
        uint64_t d1 = x % 10ULL;
        uint64_t d2 = (x / 10ULL) % 10ULL;
        if ((d1 == 6ULL && d2 == 3ULL) || (d1 == 3ULL && d2 == 6ULL))
            return true;
        x /= 10ULL;
    }
    return false;
}

/* Return the nth term of OEIS A044022, 1‑based index. */
seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t   *cache   = NULL;
    static size_t  cap      = 0;
    static size_t  len      = 0;
    static uint64_t last    = 0;   /* last number tested */

    if (n == 0)
        return (seq_t)0;          /* undefined, but avoid crash */

    /* Generate terms until we have at least n of them */
    while (len < (size_t)n) {
        uint64_t k = ++last;      /* test next integer */

        if (!contains_36_or_63(k))
            continue;            /* no required pattern */

        if (contains_36_or_63(k + 1ULL))
            continue;            /* k+1 has the pattern, skip */

        /* Store k in the cache */
        if (len == cap) {
            size_t new_cap = (cap ? cap * 2 : 16);
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();   /* memory exhausted */
            cache = tmp;
            cap   = new_cap;
        }
        cache[len++] = (seq_t)k;
    }

    return cache[(size_t)n - 1];
}
