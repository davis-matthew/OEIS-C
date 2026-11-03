/*
 * A166711  Permutation of the integers: two positives, one negative.
 * https://oeis.org/A166711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 190, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 8631 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-3) – a(n-6),  a(0)=0, a(1)=1, a(2)=2,
   a(3)=–1, a(4)=3, a(5)=4;  valid for all n ≥ 0. */

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

/* Memoised recursive evaluation using a dynamic cache. */
seq_t a(idx_t n)
{
    /* Static state for memoisation. */
    static seq_t *cache = NULL;    /* array of computed values */
    static size_t cap   = 0;       /* current allocated capacity */
    static size_t sz    = 0;       /* number of values already populated */

    /* Return cached result if already computed. */
    if (n < sz)
        return cache[n];

    /* Expand cache so that it can hold indices up to n. */
    size_t needed = (size_t)n + 1;
    if (needed > cap)
    {
        size_t newcap = cap == 0 ? 16 : cap;
        while (newcap < needed)
            newcap <<= 1;           /* double until we have enough */

        seq_t *tmp = seq_realloc(cache, cap, newcap);
        cache = tmp;
        cap = newcap;
    }

    /* Initial five values are known explicitly. */
    static const seq_t init[6] = {0, 1, 2, (seq_t)-1, 3, 4};

    /* Compute missing values up to n. */
    for (size_t i = sz; i < needed; ++i)
    {
        if (i < 6)
            cache[i] = init[(size_t)i];
        else
            cache[i] = (seq_t)2 * cache[i - 3] - cache[i - 6];
    }

    sz = needed;          /* we now have all values up to index n */
    return cache[(size_t)n];
}
