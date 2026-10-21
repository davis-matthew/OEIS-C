/*
 * A095125  Expansion of -x*(-1-x+x^2) / ( 1-2*x-3*x^2+x^3 ).
 * https://oeis.org/A095125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n‑1) + 3*a(n‑2) − a(n‑3) with * a(1)=1, a(2)=3,
 *            a(3)=8
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[i] holds a(i+1) for i ≥ 0          */
static idx_t cache_cap = 0;   /* number of valid elements in cache        */

/* --------------------------------------------------------------------- */
/* Ensure that cache contains terms a(1)…a(n) (n ≥ 1).  New elements are */
/* created and old elements are preserved.  Works for both C and C++    */
/* --------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= n)                 /* already good                */
        return;

    /* allocate a new larger array                                   */
    idx_t new_cap = n;
    seq_t *tmp = NULL;

#ifdef __cplusplus
    /* C++: use new/delete */
    tmp = new seq_t[new_cap];
    if (!tmp && new_cap > 0) abort();
#else
    /* C: use malloc/free */
    tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) abort();
#endif

    /* copy existing values */
    for (idx_t i = 0; i < cache_cap; ++i)
        tmp[i] = cache[i];

    /* free old storage */
#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif

    cache = tmp;

    /* initialise the new entries (C only needs zero‑init) */
    for (idx_t i = cache_cap; i < new_cap; ++i) {
#ifndef __cplusplus
        cache[i] = (seq_t)0;
#endif
    }

    /* set the first three terms if they are not already defined   */
    for (idx_t i = cache_cap; i < new_cap && i < 3; ++i) {
        switch (i) {
            case 0: cache[0] = (seq_t)1; break;
            case 1: cache[1] = (seq_t)3; break;
            case 2: cache[2] = (seq_t)8; break;
        }
    }

    /* compute missing terms using the recurrence                */
    for (idx_t i = cache_cap; i < new_cap; ++i) {
        if (i >= 3) {                     /* corresponds to a(i+1)   */
            cache[i] = 2*cache[i-1] + 3*cache[i-2] - cache[i-3];
        }
    }

    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Public function required by the interface                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* out of range; value is irrelevant         */
        return (seq_t)0;

    ensure_cache(n);
    return cache[n-1];   /* cache[0] holds a(1)                       */
}
