/*
 * A044492  Numbers n such that string 1,3 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search implementation for OEIS A044492
   – A number n is a member iff base‑6 representation of n
   contains the substring “13” while base‑6 of n+1 does not. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* return true if base‑6 representation of x contains the substring “13” */
static bool has13(seq_t x)
{
    seq_t prev = 0;
    bool first = true;
    while (x > 0) {
        seq_t cur = x % 6;   /* last base‑6 digit */
        x = x / 6;           /* shift right */
        if (!first) {
            /* reading digits from least‑significant to most‑significant,
               a “13” corresponds to a 1 following a 3. */
            if (prev == 3 && cur == 1)
                return true;
        }
        prev = cur;
        first = false;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t   cache_cap = 0;
    static size_t   cache_len = 0;
    static seq_t    candidate = 1;   /* next integer to test */

    /* extend cache until it contains at least n terms */
    while (cache_len < n) {
        seq_t cur = candidate;
        candidate++;

        if (has13(cur) && !has13(cur + 1)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                /* cast realloc result to seq_t* (required in C++) */
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = cur;
        }
    }
    return cache[(size_t)(n - 1)];
}
