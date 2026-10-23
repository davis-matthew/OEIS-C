/*
 * A044065  Numbers n such that string 1,0 occurs in the base 4 representation
 *          of n but not of n-1.
 * https://oeis.org/A044065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search for n such that the base‑4 representation of n contains the
   substring "10" but that of n‑1 does not. */
#include <stdlib.h>
#include <stdbool.h>

static bool has10(seq_t x)
{
    while (x >= 4) {
        if ((x % 16) == 4)            /* "10" in base‑4 == 4 decimal */
            return true;
        x /= 4;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Cache of already found terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static idx_t next_candidate = 1;   /* integer to test next */

    while (cache_len < n) {
        seq_t cur = (seq_t)next_candidate;

        if (has10(cur) && !has10(cur - 1)) {
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cur;
        }

        next_candidate++;
    }

    return cache[n - 1];
}
