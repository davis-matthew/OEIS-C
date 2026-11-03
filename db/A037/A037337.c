/*
 * A037337  Numbers whose base-9 and base-10 expansions have no digits in
 *          common.
 * https://oeis.org/A037337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 960 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑9 and base‑10 representations have no common digit */
/* Implementation: naive incremental search with memoisation */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true if the decimal digits and the base‑9 digits of x are disjoint */
static bool has_no_common_digits(seq_t x)
{
    unsigned int dec_mask  = 0;
    unsigned int base9_mask = 0;
    seq_t t = x;

    /* decimal digits */
    while (t > 0) {
        unsigned int d = (unsigned int)(t % 10);
        dec_mask |= 1u << d;
        t /= 10;
    }

    /* base‑9 digits */
    t = x;
    while (t > 0) {
        unsigned int d = (unsigned int)(t % 9);
        base9_mask |= 1u << d;
        t /= 9;
    }

    return (dec_mask & base9_mask) == 0;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Static cache of earlier terms */
    static seq_t *cache        = NULL;  /* sequence array */
    static size_t cache_len    = 0;     /* number of cached terms */
    static size_t cache_cap    = 0;     /* allocated capacity */
    static seq_t last_candidate = 0;    /* last value examined */

    if (n == 0) return 0;  /* undefined for n<1, but harmless */

    /* already have enough terms? */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Grow cache as needed */
    seq_t cand = last_candidate + 1;
    for (;;) {
        if (has_no_common_digits(cand)) {
            /* ensure capacity */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
            if (cache_len >= (size_t)n) {
                last_candidate = cand;
                break;
            }
        }
        ++cand;
    }

    return cache[n - 1];
}
