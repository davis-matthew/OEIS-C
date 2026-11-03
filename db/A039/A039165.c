/*
 * A039165  Numbers whose base-11 representation has the same number of 1's
 *          and 2's.
 * https://oeis.org/A039165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑11 representation has an equal number of 1's and 2's.
   The function enumerates integers in increasing order, keeping a cache of the
   found terms so that subsequent calls (in any order) are fast. */
#include <stdlib.h>
#include "../utils/mem.h"

static inline int digit_counts_equal(seq_t x) {
    int c1 = 0, c2 = 0;
    while (x) {
        seq_t d = x % (seq_t)11;
        if (d == (seq_t)1) c1++;
        else if (d == (seq_t)2) c2++;
        x /= (seq_t)11;
    }
    return c1 == c2;
}

seq_t a(idx_t n) {
    /* 0‑based arguments are treated as 0, but the OEIS offset is 1.
       For n==0 we simply return 0, otherwise we need the nth term. */
    if (n == 0) return (seq_t)0;

    /* static cache of generated terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* static variable holding the next integer to test */
    static seq_t next_candidate = 0;

    /* extend the cache until it contains at least n terms */
    while (cache_len < n) {
        if (digit_counts_equal(next_candidate)) {
            /* grow the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *new_arr = mem_new(seq_t, new_cap);
                for (size_t i = 0; i < cache_len; ++i) new_arr[i] = cache[i];
                mem_free(cache);
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;   /* move to the next integer */
    }

    return cache[n - 1];
}
