/*
 * A072903  Numbers m such that the discriminant of the quadratic field
 *          Q(sqrt(m)) < m.
 * https://oeis.org/A072903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   valid_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate integers m starting from 4; include m if the discriminant
   of Q(sqrt(m)) is less than m.  The discriminant depends only on the
   square‑free part m0 of m:  Δ = m0 (if m0≡1 mod 4) else Δ = 4·m0.
   Thus Δ < m ⇔ (m0≡1 mod 4 and k≥2) or (m0≠1 mod 4 and k≥3), where
   m = m0·k².  This function uses memoisation so that calls with arbitrary
   n are O(n) after the largest n requested. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/valid_m.h"

/* Static memoisation table.  seq_cache holds the terms a(1), a(2), … */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;   /* allocated slots in seq_cache */
static idx_t cache_len = 0;    /* number of terms stored so far */
static uint64_t next_m = 4;    /* next integer to test for validity */

seq_t a(idx_t n)
{
    if (n == 0)        /* the sequence is offset 1; return 0 for safety */
        return 0;

    /* Grow the cache until it contains the n‑th term. */
    while (cache_len < n) {
        /* Allocate or grow the storage as needed.  The cast makes it
           compatible with both C and C++ where seq_t may be a class. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();            /* unrecoverable memory failure */
            seq_cache = tmp;
            cache_cap = new_cap;
        }

        /* Test the current integer. */
        if (valid_m(next_m)) {
            seq_cache[cache_len++] = (seq_t)next_m;
        }
        ++next_m;
    }

    return seq_cache[n - 1];
}
