/*
 * A043827  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 2 mod 3.
 * https://oeis.org/A043827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th number k such that the number of runs
   in the base‑4 representation of k is ≡ 2 (mod 3).  It uses a static
   cache, growing it as needed.  The first few values are
   4, 6, 7, 8, 9, 11, 12, … . */
#pragma once
#include <stdlib.h>
#include <stddef.h>

/* Helper: count runs in base‑4 representation of v. */
static inline unsigned base4_runs(seq_t v) {
    unsigned runs = 0;
    unsigned prev = 4; /* sentinel not a valid digit */
    while (v != 0) {
        unsigned d = (unsigned)(v % 4); /* digit in [0,3] */
        if (d != prev) {
            ++runs;
            prev = d;
        }
        v = v / 4;
    }
    return runs;
}

seq_t a(idx_t n) {
    /* 0‑based cache of terms that satisfy the condition. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static idx_t test_next = 1; /* next integer to test (starting at 1) */

    if (n == 0) return (seq_t)0; /* undefined offset, return 0 */

    /* Grow cache while it does not contain at least n terms. */
    while (cache_size < (size_t)n) {
        /* Test test_next and store if it satisfies the runs condition. */
        if (base4_runs((seq_t)test_next) % 3 == 2) {
            if (cache_cap == 0) {
                cache_cap = 64;
                cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            } else if (cache_size >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)test_next;
        }
        ++test_next;
    }

    return cache[(size_t)n - 1];
}
