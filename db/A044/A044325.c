/*
 * A044325  Numbers k such that the string 8,2 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 889, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9937 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044325: numbers k such that the base‑9 representation of k
   contains the consecutive digits 8 followed by 2, but the base‑9
   representation of k-1 does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper to test whether a number has the substring "82" in its
   base‑9 representation. */
static inline bool has_82(seq_t x)
{
    if (x < 9) return false;
    seq_t last = x % 9;
    x /= 9;
    while (x > 0) {
        seq_t cur = x % 9;
        if (cur == 8 && last == 2) return true;
        last = cur;
        x /= 9;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation across calls. */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;
    static seq_t cur_k = 1;          /* last integer examined */
    static bool prev_has = false;    /* has_82 for cur_k - 1 */

    /* Grow the cache array as needed. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Ensure we've found at least n terms. */
    while (cache_sz < (size_t)n) {
        cur_k++;                             /* examine next integer */
        bool cur_has = has_82(cur_k);
        if (cur_has && !prev_has) {
            /* New qualifying term: add to cache. */
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *newp = mem_resize(seq_t, cache, new_cap);
                if (newp == NULL) {           /* allocation failure */
                    mem_free(cache);
                    cache = NULL;
                    cache_sz = 0;
                    cache_cap = 0;
                    return 0;                 /* undefined: cannot continue */
                }
                cache = newp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = cur_k;
        }
        prev_has = cur_has;                  /* update for next step */
    }

    return cache[n - 1];
}
