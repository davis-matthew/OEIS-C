/*
 * A097269  Numbers that are the sum of two nonzero squares but not the
 *          difference of two nonzero squares.
 * https://oeis.org/A097269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   odd_good
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097269 – Numbers that are the sum of two non‑zero squares
   but not the difference of two non‑zero squares.  The function
   generates terms by traversing odd integers m, checking whether m
   has any prime factor congruent to 3 (mod 4) to an odd power; if so
   m fails.  For each acceptable m the term 2·m is stored.  The cache
   is expanded lazily, so any call to a(n) returns the correct value
   irrespective of call order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/odd_good.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* behaviour below offset is irrelevant */

    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  next_candidate = 3;   /* next odd number to test */

    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        idx_t m;
        if (cache_len == 0) {
            m = 1;                          /* first odd number */
        } else {
            /* Find the next good odd m. */
            do {
                if (odd_good(next_candidate)) {
                    m = next_candidate;
                    next_candidate += 2;    /* advance past chosen m */
                    break;
                }
                next_candidate += 2;
            } while (1);
        }

        seq_t val = (seq_t)2 * (seq_t)m;     /* 2·m is the required term */

        if (cache_len >= cache_cap) {
            cache_cap <<= 1;                /* double size */
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }
        cache[cache_len++] = val;
    }

    return cache[n - 1];
}
