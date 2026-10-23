/*
 * A043668  Numbers whose base-14 representation has exactly 7 runs.
 * https://oeis.org/A043668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer whose base‑14 representation contains
 *            exactly seven runs. Computed by brute‑force enumeration of
 *            successive integers starting from 14^6
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   has7runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has7runs.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate;
    static bool initialized = false;

    if (!initialized) {
        /* Initial capacity is enough for the first few terms. */
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* Smallest 7‑digit base‑14 number: 14^6. */
        seq_t val = 1;
        for (int i = 0; i < 6; ++i) val *= 14;
        next_candidate = val;
        initialized = true;
    }

    while (cache_len < n) {
        /* Find the next integer with exactly seven runs. */
        while (!has7runs(next_candidate))
            ++next_candidate;

        /* Store it.  Grow the array if necessary. */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
