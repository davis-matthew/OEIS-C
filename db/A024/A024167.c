/*
 * A024167  a(n) = n!*(1 - 1/2 + 1/3 - ... + c/n), where c = (-1)^(n+1).
 * https://oeis.org/A024167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) * (n-1)^2 (n > 2) with a(1) = 1, a(2) =
 *            1. This is the recurrence given in the OEIS entry
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 437 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Cache of already finished values, 1‑based indexing */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;      /* allocated array size (>= n+1) */
    static idx_t   max_n = 0;      /* largest n for which cache is valid */

    /* The sequence is defined for n≥1; otherwise return 0 (unused). */
    if (n == 0)
        return (seq_t)0;

    /* Initialisation: allocate a small backing array and fill bases */
    if (cache == NULL) {
        cap = 4;                /* minimal size (index 0..3) */
        cache = seq_alloc(cap); /* allocate sequence array */
        cache[0] = (seq_t)0;    /* unused slot to keep 1‑based scheme */
        cache[1] = (seq_t)1;    /* a(1) */
        cache[2] = (seq_t)1;    /* a(2) */
        max_n      = 2;
    }

    /* Grow the cache if the requested n lies beyond the current limit */
    if (n > max_n) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;   /* double capacity until it fits n+1 elements */

        cache = seq_realloc(cache, cap, new_cap);
        cap   = new_cap;

        /* Compute the missing terms, iteratively using the recurrence. */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* (i-1)^2 fits in seq_t after casting i-1 to seq_t. */
            seq_t sq   = (seq_t)(i - 1) * (seq_t)(i - 1);
            seq_t prod = cache[i - 2] * sq;
            cache[i]   = cache[i - 1] + prod;
        }
        max_n = n;
    }

    return cache[n];
}
