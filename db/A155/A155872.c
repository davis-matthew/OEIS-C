/*
 * A155872  a(n) = 10^n + 11^n.
 * https://oeis.org/A155872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) - 110*a(n-2), a(0)=2, a(1)=21
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;      /* array holding a(0)…a(k) */
    static size_t cap = 0;           /* allocated number of slots */
    static idx_t last = (idx_t)-1;   /* index of the last computed term */

    /* Initialisation on first call */
    if (cache == NULL) {
        /* Allocate space for first two terms */
        size_t init_cap = 8;                 /* small enough for most tests */
        cache = seq_alloc_array(init_cap);
        if (cache == NULL) return (seq_t)0;  /* allocation failure */

        cap = init_cap;
        cache[0] = (seq_t)2;      /* a(0) */
        cache[1] = (seq_t)21;     /* a(1) */
        last    = 1;
    }

    /* Return immediately if n is already cached */
    if (n <= last) {
        return cache[n];
    }

    /* Ensure that we have enough space for index n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 8;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;          /* double until large enough */
        }
        seq_t *new_cache = seq_alloc_array(new_cap);
        if (new_cache == NULL) {
            /* Allocation failure – behave as if zero */
            return (seq_t)0;
        }
        /* Copy old values */
        for (size_t i = 0; i <= (size_t)last; ++i) {
            new_cache[i] = cache[i];
        }
        /* Free old cache */
        seq_free_array(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = (seq_t)21 * cache[i - 1] - (seq_t)110 * cache[i - 2];
        cache[i] = term;
    }
    last = n;
    return cache[n];
}
