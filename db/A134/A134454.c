/*
 * A134454  Numbers m such that the digital root in balanced ternary
 *          representation equals +1.
 * https://oeis.org/A134454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bal_digit_sum, bal_root_plus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute Numbers m such that the digital root in balanced ternary
    representation equals +1.
    The sequence is built by scanning successive integers and testing
    the digital‑root property.  A small cache is kept so that the
    function works for arbitrary orders of calls.
    Formula: a(n) = m where m is the n‑th number whose balanced ternary
    digital root is +1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bal_digit_sum.h"
#include "../utils/bal_root_plus_one.h"

/* Sequence function: a(idx_t n) */
seq_t a(idx_t n)
{
    /* Cache of generated terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_val = 0;   /* largest term already stored */

    /* Allocate cache on first call. */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* unlikely, but defensive */
    }

    /* Return cached value if available. */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Extend cache until reaching the requested index. */
    seq_t cand = last_val;           /* start scanning after last value */
    while (cache_len < n) {
        cand = cand + 1;             /* next integer to test */
        if (bal_root_plus_one(cand)) {
            /* Grow cache if necessary. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0; /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
            last_val = cand;
        }
    }

    return cache[n - 1];
}
