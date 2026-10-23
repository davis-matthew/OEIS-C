/*
 * A039591  Numbers whose base-6 representation has the same number of 2's,
 *          4's and 5's.
 * https://oeis.org/A039591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A039591: Numbers whose base-6 representation has the same
   number of 2's, 4's and 5's.  Computed by incremental search. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"

/* Cached sequence data */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 0;

/* Test whether a number satisfies the digit‑count condition */
static bool good(seq_t x)
{
    unsigned cnt2 = 0, cnt4 = 0, cnt5 = 0;
    seq_t t = x;
    while (t > 0) {
        unsigned d = (unsigned)(t % 6);
        if (d == 2) ++cnt2;
        else if (d == 4) ++cnt4;
        else if (d == 5) ++cnt5;
        t /= 6;
    }
    return (cnt2 == cnt4) && (cnt4 == cnt5);
}

/* Extend the cache until it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        if (good(next_candidate)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0 ? 16 : cache_cap * 2);
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* OEIS A039591: nth term, 1‑indexed */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* guard against zero index */
    ensure_cache(n);
    return cache[n - 1];
}
