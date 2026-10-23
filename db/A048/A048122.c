/*
 * A048122  a(1) = 1, a(2) = 2, a(n) = a(n-1)^3 - a(n-2)^3.
 * https://oeis.org/A048122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 2, a(n) = a(n-1)^3 - a(n-2)^3 (n ≥ 3)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdlib.h>

/*  seq_t and idx_t are defined by the build system.  */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t stored  = 0;   /* number of terms stored (cache[0] == a(1)) */

    /* Accept only positive indices.  Undefined for n == 0. */
    if (n == 0) return (seq_t)0;

    /* Ensure we can hold at least n terms. */
    if (n > capacity) {
        size_t new_cap = capacity ? capacity : 1;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialize first terms when needed. */
    if (stored == 0) {            /* a(1) */
        cache[0] = (seq_t)1;
        stored = 1;
    }
    if (stored == 1 && n >= 2) {   /* a(2) */
        cache[1] = (seq_t)2;
        stored = 2;
    }

    /* Compute missing terms up to index n (0‑based: a(n) -> cache[n‑1]). */
    while (stored < (size_t)n) {
        /* indices for the recurrence: cache[stored] = a(stored+1) */
        seq_t a_prev  = cache[stored - 1];           /* a(stored)   */
        seq_t a_prev2 = cache[stored - 2];           /* a(stored-1) */
        /* cube the terms:   a_prev^3  and  a_prev2^3 */
        seq_t cube1 = a_prev * a_prev * a_prev;
        seq_t cube2 = a_prev2 * a_prev2 * a_prev2;
        cache[stored] = cube1 - cube2;
        ++stored;
    }

    return cache[n - 1];
}
