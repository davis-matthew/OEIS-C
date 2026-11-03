/*
 * A043765  Numbers n such that number of runs in the base 2 representation of
 *          n is congruent to 2 mod 10.
 * https://oeis.org/A043765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 987 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th positive integer whose binary representation
   has a number of runs that is congruent to 2 modulo 10. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Count whether a given number has the required run property. */
static bool passes_seq_t(seq_t x)
{
    seq_t runs = 1;                 /* at least one run */
    int    prev = (int)(x & 1);     /* lowest bit */
    x >>= 1;

    while (x != 0) {
        int bit = (int)(x & 1);
        if (bit != prev) {
            runs++;
            prev = bit;
        }
        x >>= 1;
    }

    return ((runs % (seq_t)10) == (seq_t)2);
}

seq_t a(idx_t n)
{
    /* Static cache of previously found terms. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated slots */
    static size_t len   = 0;   /* number of terms stored */
    static seq_t next_candidate = 1;   /* number to start searching from */

    /* Initial allocation. */
    if (!cache) {
        cap = 128;
        cache = mem_new(seq_t, cap);
        if (!cache) abort();   /* out of memory */
        len = 0;
    }

    /* Find terms until we have at least n of them. */
    while (len < n) {
        seq_t cand = next_candidate;
        do {
            cand++;
        } while (!passes_seq_t(cand));

        /* Grow cache if necessary. */
        if (len == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();   /* out of memory */
            cache = tmp;
            cap   = new_cap;
        }

        cache[len++] = cand;
        next_candidate = cand + 1;
    }

    return cache[n - 1];
}
