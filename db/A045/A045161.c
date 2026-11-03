/*
 * A045161  Numbers whose base-5 representation contains exactly one 0 and no
 *          1's.
 * https://oeis.org/A045161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 4774, u32 n <= 7798, u64 n <= 7798,
 *            u128 n <= 7798, big any n
 * Validated: standard, pass, 66 terms through n = 7651 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base-5 representation contains exactly one 0 and no 1's. */

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a value has exactly one 0 in base‑5 and no digit 1. */
static bool valid_base5_onezero_noone(seq_t v)
{
    if (v == 0)          /* representation "0" has one zero, no 1's */
        return true;

    seq_t cnt = 0;
    seq_t tmp = v;
    while (tmp > 0) {
        seq_t d = tmp % 5;          /* base‑5 digit */
        if (d == 1)                 /* forbidden digit 1 */
            return false;
        if (d == 0)                 /* count zero digits */
            cnt++;
        tmp /= 5;
    }
    return cnt == 1;                 /* exactly one zero required */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Cached values */
    static seq_t *cache = NULL;      /* array of cached terms */
    static size_t cache_cap = 0;     /* allocated capacity of cache */
    static size_t cache_len = 0;     /* number of terms stored */
    static idx_t next_start = 0;     /* next integer to examine */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
        assert(cache);
        cache_len = 0;
        next_start = 0;
    }

    /* Populate cache until it contains at least n terms */
    while (cache_len < n) {
        seq_t candidate = (seq_t)next_start;
        next_start++;

        if (valid_base5_onezero_noone(candidate)) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *new_cache = mem_new(seq_t, new_cap);
                assert(new_cache);
                for (size_t i = 0; i < cache_len; ++i)
                    new_cache[i] = cache[i];
                mem_free(cache);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
    }

    return cache[n - 1];                /* a(1) is stored at index 0 */
}
