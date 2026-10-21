/*
 * A207455  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 10*a(n-2) + 3*a(n-3) - 9*a(n-4) (empirical
 *            recurrence) a(1) = 8, a(2) = 64, a(3) = 168, a(4) = 760 Offset =
 *            1 This implementation memoises the sequence so that any order of
 *            calls is handled efficiently
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  a(n):  number of 4×n 0/1 arrays avoiding the forbidden patterns   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;      /* cache[0] unused, indices start at 1   */
    static size_t cap    = 0;          /* allocated capacity of cache array     */
    static size_t sz     = 0;          /* highest n computed so far (>=0)       */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 5;                       /* indices 0..4 → 5 elements            */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure – unlikely */
        cache[0] = (seq_t)0;           /* unused */
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)64;
        cache[3] = (seq_t)168;
        cache[4] = (seq_t)760;
        sz = 4;
    }

    /* The sequence is defined for n ≥ 1 only.  For any n < 1 return 0. */
    if (n < 1) return (seq_t)0;

    /* If we already have the value, return it directly. */
    if (n <= (idx_t)sz)
        return cache[(size_t)n];

    /* Grow the cache so that it can hold indices up to n. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap == 0 ? 4 : cap;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;                       /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;        /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = sz + 1; i <= n; ++i) {
        seq_t a1 = cache[(size_t)(i-1)];
        seq_t a2 = cache[(size_t)(i-2)];
        seq_t a3 = cache[(size_t)(i-3)];
        seq_t a4 = cache[(size_t)(i-4)];
        seq_t val = a1 + (seq_t)10 * a2 + (seq_t)3 * a3 - (seq_t)9 * a4;
        cache[(size_t)i] = val;
    }
    sz = (size_t)n;
    return cache[(size_t)n];
}
