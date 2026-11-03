/*
 * A044212  Numbers n such that string 3,1 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044212
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

/* OEIS A044212
 * Numbers n such that string "31" occurs in the base‑8 representation of n
 * but not of n-1.  This function returns the nth such number.
 * It is implemented by incremental search with memoisation.
 */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool has31_oct(seq_t v)
{
    // Return true if the base‑8 representation of v contains the
    // consecutive digits 3 followed by 1.
    seq_t prev = 8;              // sentinel outside 0..7
    while (v != 0) {
        seq_t d = v % 8;
        v /= 8;
        if (d == 3 && prev == 1) return true;
        prev = d;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           // offset starts at 1

    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t  last_checked = 0;
    static bool   prev_has_last = false;

    /* initialise cache the first time */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* generate terms until we have the requested one */
    while (cache_len < n) {
        idx_t candidate = last_checked + 1;
        seq_t val = (seq_t)candidate;
        bool cur_has = has31_oct(val);

        if (cur_has && !prev_has_last) {   /* n-1 had no "31", n does */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) { mem_free(cache); return 0; }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }

        prev_has_last = cur_has;
        last_checked  = candidate;
    }

    return cache[n-1];
}
