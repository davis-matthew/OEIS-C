/*
 * A041410  Numerators of continued fraction convergents to sqrt(220).
 * https://oeis.org/A041410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041410: numerators of the convergents to sqrt(220)
   Recurrence: a(n) = 178*a(n-4) - a(n-8)
   Initial terms: a(0)=14, a(1)=15, a(2)=74, a(3)=89,
                  a(4)=2566, a(5)=2655, a(6)=13186, a(7)=15841   */

#include <stdlib.h>  /* malloc, realloc */

static const unsigned long long init_vals[8] = {
    14ULL, 15ULL, 74ULL, 89ULL,
    2566ULL, 2655ULL, 13186ULL, 15841ULL
};

seq_t a(idx_t n)
{
    /* Persistent cache for memoisation */
    static seq_t *cache     = NULL;  /* cache of computed terms */
    static size_t  cap      = 0;     /* current allocated capacity (>= 1) */
    static size_t  last     = 0;     /* index of the highest computed value */

    size_t idx = (size_t)n;          /* index to operate with */

    /* Allocate / grow cache on first call or when more space is needed */
    if (!cache) {
        /* Ensure enough space for the first 8 terms at least */
        cap = (idx + 1 > 8) ? (idx + 1) : 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;       /* allocation failed – return zero */
        /* Initialise the first eight terms (values truncated to seq_t) */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = (seq_t)init_vals[i];
        last = 7;                   /* highest index that is valid now */
    } else if (idx >= cap) {
        /* Grow the cache to accommodate the requested index */
        size_t new_cap = (idx + 1 > 8) ? (idx + 1) : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;         /* reallocation failed – return zero */
        cache = tmp;
        cap   = new_cap;
        /* (The first 8 entries are already initialised) */
    }

    /* If the term has already been computed, return it immediately */
    if (idx <= last)
        return cache[idx];

    /* Compute missing terms up to the requested index */
    for (size_t i = last + 1; i <= idx; ++i) {
        if (i < 8)
            cache[i] = (seq_t)init_vals[i];
        else
            cache[i] = ((seq_t)178 * cache[i - 4]) - cache[i - 8];
    }

    last = idx;
    return cache[idx];
}
