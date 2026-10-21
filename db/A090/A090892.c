/*
 * A090892  Solutions x to equation floor(x*r*floor(x/r)) =
 *          floor(x/r*floor(x*r)) when r = sqrt(2).
 * https://oeis.org/A090892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   even_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation:  a(n) enumerates the non‑negative integers k such that
    floor(k*sqrt(2)) is even; a(0) = 0, a(1) = 1.  The function
    generates terms on demand, memoising them so that calls in any order
    stay fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/even_parity.h"

seq_t a(idx_t n)
{
    /* memoisation storage -------------------------------------------- */
    static seq_t *cache          = NULL;
    static size_t cache_cap      = 0;
    static idx_t  last_processed = 1;  /* we have processed k = 1 at start */
    static idx_t  terms_count    = 2;  /* indices 0 and 1 already stored */

    /* initialise the cache on first call ----------------------------- */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;      /* a(0) */
        cache[1] = (seq_t)1;      /* a(1) */
    }

    /* return immediately for the first two terms --------------------- */
    if (n < terms_count) {
        return cache[n];
    }

    /* generate new terms until we have index n ----------------------- */
    idx_t candidate = last_processed + 1;   /* next k to test */
    while (terms_count <= n) {
        bool ok;
        if (candidate == 1) {
            ok = true;          /* a(1) is always 1, handled at init */
        } else {
            ok = even_parity(candidate);
        }

        if (ok) {
            if (terms_count == cache_cap) {
                cache_cap <<= 1;                /* grow by factor 2 */
                cache = (seq_t *)realloc(cache,
                                          cache_cap * sizeof(seq_t));
            }
            cache[terms_count++] = (seq_t)candidate;
        }

        ++candidate;
    }

    last_processed = candidate - 1;
    return cache[n];
}
