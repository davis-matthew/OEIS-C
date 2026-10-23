/*
 * A037583  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1.
 * https://oeis.org/A037583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (13*4^n - 3*(-1)^n - 10)/15
 * Recurrence (n ≥ 4): a(n) = 4*a(n-1) + a(n-2) - 4*a(n-3)
 * Initial terms: a(1)=3, a(2)=13, a(3)=55
 *
 * Computes the n‑th term of OEIS sequence A037583.
 * The implementation uses a static cache to support calls
 * in arbitrary order and keeps all arithmetic in seq_t.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Result for indices below the offset is irrelevant; return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache: cache[i] holds a(i) for i >= 1. */
    static seq_t *cache = NULL;      /* pointer to dynamically allocated array */
    static idx_t  cache_len = 0;     /* largest index that has been computed so far */
    static size_t cache_cap = 0;     /* current allocated capacity (≥ cache_len + 1) */

    /* Ensure the array can hold index n (1‑based).  +1 for 0‑unused slot. */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        /* Grow capacity to required, but avoid frequent reallocs by doubling. */
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < required)
            new_cap = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the first three terms if they are not yet set. */
    if (cache_len < 1) {
        cache[1] = (seq_t)3;   /* a(1) */
        cache_len = 1;
    }
    if (cache_len < 2) {
        cache[2] = (seq_t)13;  /* a(2) */
        cache_len = 2;
    }
    if (cache_len < 3) {
        cache[3] = (seq_t)55;  /* a(3) */
        cache_len = 3;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Recurrence: a(i) = 4*a(i-1) + a(i-2) - 4*a(i-3) */
        seq_t term = 4 * cache[i - 1] + cache[i - 2] - 4 * cache[i - 3];
        cache[i] = term;
    }
    cache_len = n;

    return cache[n];
}
