/*
 * A121966  a(n) = a(n-1) - (n-1)*a(n-2), with a(0) = 1, a(1) = 2.
 * https://oeis.org/A121966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) - (n-1)*a(n-2), a(0)=1, a(1)=2
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 20, i64 n <= 33, i128 n <= 57,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

/* Cached sequence values: cache[i] holds a(i) */
static seq_t *cache = NULL;
static idx_t   cap   = 0;   /* total capacity (number of slots) */
static idx_t   last  = 0;   /* index of the last computed term */

/* Ensure that cache can hold terms up to 'needed' (inclusive). */
static void grow_cache(idx_t needed)
{
    if (cap >= needed)
        return;

    idx_t new_cap = cap ? cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *newc = alloc_seq_array((size_t)new_cap);
    if (!newc)            /* allocation failure – unlikely in the test harness */
        return;

    if (cache) {
        for (idx_t i = 0; i <= last; ++i)
            newc[i] = cache[i];
        free(cache);
    }
    cache = newc;
    cap   = new_cap;
}

/* Compute a(n) from a(n-1) and a(n-2).  All operations stay in type seq_t. */
static inline seq_t compute_next(seq_t prev, seq_t prev2, idx_t k)
{
    /* k is (n-1).  Cast to seq_t so the multiplication uses the seq_t
       overload in C++ or the built‑in arithmetic in C.  This works for
       both the fixed‑size and arbitrary‑precision builds. */
    return prev - (seq_t)k * prev2;
}

seq_t a(idx_t n)
{
    /* initialise base cases once */
    if (last == 0 && cap < 2) {
        grow_cache(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        last = 1;
    }

    if (n <= last)
        return cache[n];

    grow_cache(n + 1);

    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = compute_next(cache[i-1], cache[i-2], i-1);
    }
    last = n;
    return cache[n];
}
