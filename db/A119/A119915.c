/*
 * A119915  Number of ternary words of length n and having exactly one run of
 *          0's of odd length.
 * https://oeis.org/A119915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [z^n] z*(1 - z^2)/(1 - 2*z - z^2)^2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational form: */
/* a(0)=0, a(1)=1, a(2)=4, a(3)=13, and for n≥4 */
/* a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) - a(n-4) */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array holding computed a(k) terms */
static size_t cache_size = 0;        /* number of entries currently stored */
static size_t cache_capacity = 0;    /* current allocated array capacity */

/* Ensure that cache contains values up to a(n).  Expands capacity as needed. */
static void ensure_cache(idx_t n)
{
    /* if we already have the requested index stored, nothing to do */
    if (cache_size > (size_t)n) return;

    /* --- Allocate/expand array --- */
    size_t needed = (size_t)n + 1;          /* indices 0..n inclusive */

    if (cache_capacity < needed) {
        size_t new_cap = cache_capacity ? cache_capacity : 4;  /* start small */
        while (new_cap < needed) new_cap <<= 1;                 /* grow by 2x */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                                 /* OOM */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* --- Initialize base values if this is the first call --- */
    if (cache_size == 0) {
        cache[0] = 0;   /* a(0) */
        cache[1] = 1;   /* a(1) */
        cache[2] = 4;   /* a(2) */
        cache[3] = 13;  /* a(3) */
        cache_size = 4;
    }

    /* --- Compute missing terms up to a(n) --- */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* for i < 4 values already set, for i ≥ 4 compute by recurrence */
        if (i < 4) continue;
        cache[i] = 4 * cache[i-1]
                   - 2 * cache[i-2]
                   - 4 * cache[i-3]
                   - cache[i-4];
    }

    /* mark number of elements stored */
    cache_size = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
