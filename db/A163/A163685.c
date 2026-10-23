/*
 * A163685  Number of n X 2 binary arrays with all 1s connected, a path of 1s
 *          from upper left corner to lower right corner, and no 1 having more
 *          than two 1s adjacent.
 * https://oeis.org/A163685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-4) for n >= 7 Initial values:
 *            a(1)=1, a(2)=3, a(3)=5, a(4)=8, a(5)=13, a(6)=20 (All values fit
 *            in the given seq_t type or are handled exactly by the
 *            arbitrary-precision build.)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc, free, exit */

static seq_t *cache = NULL;   /* array of computed terms, index 1..n */
static size_t cache_cap = 0;  /* allocated size of cache (bytes) */
static idx_t  cache_nmax = 0; /* largest n for which cache[n] is valid */

/* Ensure that cache has room for indices 0..new_cap-1.  Handles both C
   and C++ builds. */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

#if defined(__cplusplus)
    /* Allocate new array, copy existing values, delete old one. */
    seq_t *new_cache = new seq_t[new_cap];
    /* Copy existing elements.  For types without trivial copy ctor,
       this will invoke the copy constructor. */
    for (size_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
    cache = new_cache;
    cache_cap = new_cap;
#else
    /* Use realloc for C.  C++ builds should not hit this branch. */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);  /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
#endif
}

/* Compute a(n) for the OEIS sequence A163685. */
seq_t a(idx_t n)
{
    if (n < 1)          /* behaviour for n < 1 is irrelevant */
        return (seq_t)0;

    /* We always keep index 0 unused, so we need n+1 slots.  For n<6
       we also want room for the initial values (up to 6). */
    size_t needed = (size_t)n + 1;
    if (needed < 7) needed = 7;
    ensure_capacity(needed);

    /* Initialise the first six terms once. */
    if (cache_nmax < 6) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)8;
        cache[5] = (seq_t)13;
        cache[6] = (seq_t)20;
        cache_nmax = 6;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_nmax + 1; i <= n; ++i) {
        if (i <= 6) continue;    /* should already be set */

        seq_t val = cache[(size_t)(i - 1)] + cache[(size_t)(i - 1)];  /* 2*a(i-1) */
        val -= cache[(size_t)(i - 2)];                                 /* -a(i-2) */
        val += cache[(size_t)(i - 4)];                                 /* +a(i-4) */
        cache[(size_t)i] = val;
    }

    cache_nmax = n > cache_nmax ? n : cache_nmax;
    return cache[(size_t)n];
}
