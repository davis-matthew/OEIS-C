/*
 * A160682  The list of the A values in the common solutions to 13*k+1 = A^2
 *          and 17*k+1 = B^2.
 * https://oeis.org/A160682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 14, a(n) = 15*a(n-1) - a(n-2) for n > 2 The
 *            sequence is defined by a second‑order linear recurrence with
 *            integer coefficients. This implementation uses a static cache so
 *            that any order of calls works efficiently
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 184 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

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

/* Compute the n-th term (1‑based).  The function is memoised
   internally so subsequent calls with the same or larger n are
   O(1) in the differences. */
seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i+1)  (i = 0 => a(1)) */
    static seq_t *cache = NULL;
    static idx_t cap     = 0;      /* allocated size (number of elements) */
    static idx_t computed = 0;     /* how many elements are already filled */

    /* allocate initial cache if necessary */
    if (cache == NULL) {
        cache = seq_alloc(4);
        cap   = 4;
    }

    /* grow cache if requested n is larger than current capacity */
    if ((idx_t)n > cap) {
        idx_t newcap = cap ? cap << 1 : 4;
        while (newcap < n) newcap <<= 1;
        seq_t *tmp = seq_realloc(cache, cap, newcap);
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms up to n */
    if (computed < n) {
        for (idx_t i = computed; i < n; ++i) {
            if (i == 0) {
                cache[i] = 1;          /* a(1) */
            } else if (i == 1) {
                cache[i] = 14;         /* a(2) */
            } else {
                cache[i] = cache[i-1] * 15 - cache[i-2];
            }
        }
        computed = n;
    }

    return cache[n-1];
}
