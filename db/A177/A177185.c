/*
 * A177185  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=10, k=-1 and l=-1.
 * https://oeis.org/A177185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(0) = 1, a(1) = 10
 *   For n ≥ 2:
 *     a(n) = Σ_{p=0}^{n-1} a(p) * a(n-1-p)  –  n  –  1
 *   (This follows from a(n+1) = Σ_{p=0}^{n} (a(p)*a(n-p)+k) + l
 *    with k = -1, l = -1.)
 */

#include <stddef.h>                /* size_t */
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

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Static cache for memoized values */
    static seq_t *cache   = NULL;
    static idx_t  cached  = 0; /* largest index whose value is in cache */
    static size_t  cap    = 0; /* allocated capacity of the cache */

    /* Initialisation on first call */
    if (cache == NULL) {
        /* Allocate capacity for indices 0 and 1 */
        size_t init_cap = 2;
        cache = seq_alloc(init_cap);      /* seq_alloc returns a seq_t* */
        if (cache == NULL) abort();       /* sanity check */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cap     = init_cap;
        cached  = 1;
    }

    /* If the requested value is already cached, return it */
    if (n <= cached) return cache[n];

    /* Grow the cache if needed */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1);
        seq_t *tmp = seq_realloc(cache, cap, new_cap);  /* reallocates properly for seq_t */
        if (tmp == NULL) abort();                       /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t m = cached + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t p = 0; p < m; ++p) {
            idx_t q = m - 1 - p;
            seq_t prod = cache[p] * cache[q];
            sum += prod;
        }
        /* a(m) = sum - m - 1 */
        sum -= (seq_t)(m) + (seq_t)1;
        cache[m] = sum;
    }

    cached = n;
    return cache[n];
}
