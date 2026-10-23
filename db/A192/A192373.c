/*
 * A192373  Constant term in the reduction of the polynomial p(n,x) defined at
 *          A162517 and below in Comments.
 * https://oeis.org/A192373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+10*a(n-2)-6*a(n-3)-9*a(n-4) with a(1)=1, a(2)=0,
 *            a(3)=7, a(4)=8
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *CACHE    = NULL;   /* array of already computed terms   */
static size_t CACHE_CAP = 0;     /* current capacity of CACHE         */
static size_t COMPUTED  = 0;     /* number of stored terms (>=0)      */

/* Reallocate the cache to hold at least new_cap elements.
   Handles C and C++ builds. */
static seq_t *realloc_cache(seq_t *old, size_t old_cap, size_t new_cap)
{
#if defined(__cplusplus)
    /* In C++ use operator new[], which default‑constructs elements. */
    seq_t *new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_cache[i] = old[i];
    if (old)
        delete [] old;
    return new_cache;
#else
    /* In C use realloc, which leaves the contents unchanged. */
    return (seq_t *)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Ensure that CACHE contains terms up to index n (1‑based). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= COMPUTED)
        return;                         /* already have enough terms */

    /* Grow capacity if required (power‑of‑two growth). */
    size_t req = (size_t)n;
    if (req > CACHE_CAP) {
        size_t new_cap = CACHE_CAP ? CACHE_CAP : 4;
        while (new_cap < req)
            new_cap <<= 1;
        seq_t *new_cache = realloc_cache(CACHE, CACHE_CAP, new_cap);
        if (!new_cache)
            abort();                     /* memory allocation failed   */
        /* Zero‑initialize the newly added slots. */
        for (size_t i = CACHE_CAP; i < new_cap; ++i)
            new_cache[i] = (seq_t)0;
        CACHE = new_cache;
        CACHE_CAP = new_cap;
    }

    /* Compute missing terms. */
    for (size_t i = COMPUTED; i < req; ++i) {
        idx_t idx = (idx_t)(i + 1); /* 1‑based index of this element */
        if (idx == 1)
            CACHE[i] = (seq_t)1;
        else if (idx == 2)
            CACHE[i] = (seq_t)0;
        else if (idx == 3)
            CACHE[i] = (seq_t)7;
        else if (idx == 4)
            CACHE[i] = (seq_t)8;
        else {
            seq_t a1 = CACHE[i - 1]; /* a(n-1) */
            seq_t a2 = CACHE[i - 2]; /* a(n-2) */
            seq_t a3 = CACHE[i - 3]; /* a(n-3) */
            seq_t a4 = CACHE[i - 4]; /* a(n-4) */
            CACHE[i] = 2 * a1 + 10 * a2 - 6 * a3 - 9 * a4;
        }
    }
    COMPUTED = req;
}

seq_t a(idx_t n)
{
    if (n == 0)                     /* sequence is defined only for n>=1   */
        return (seq_t)0;
    ensure_cache(n);
    return CACHE[(size_t)n - 1];
}
