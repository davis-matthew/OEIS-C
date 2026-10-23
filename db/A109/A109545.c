/*
 * A109545  a(n) = 2*a(n-1) + a(n-2) + a(n-3).
 * https://oeis.org/A109545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + a(n-3) a(0) = 1, a(1) = 1, a(2) = 2
 *            (Recurrence defined for n ≥ 3)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached values */
    static idx_t cache_sz = 0;      /* number of stored values */
    static size_t cap     = 0;      /* allocated capacity in slots */

    /* --- initialise cache for the first three terms --- */
    if (cache == NULL) {
        /* Allocate space for three values (capacity 3) */
        cap = 3;
        cache = alloc_seq_array(cap);
        if (cache == NULL)
            abort(); /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache_sz = 3;
    }

    /* Return cached value if already computed */
    if (n < cache_sz)
        return cache[n];

    /* --- ensure the underlying array is large enough --- */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (cap > 1) ? cap * 2 : 1;
        while ((size_t)(n + 1) > new_cap)
            new_cap *= 2;

        seq_t *new_cache = alloc_seq_array(new_cap);
        if (new_cache == NULL)
            abort(); /* allocation failure */

        for (idx_t i = 0; i < cache_sz; ++i)
            new_cache[i] = cache[i];

        free_seq_array(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* --- compute missing terms up to n --- */
    for (idx_t i = cache_sz; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-1) + a(i-2) + a(i-3) */
        cache[i] = (cache[i - 1] << 1) + cache[i - 2] + cache[i - 3];
    }
    cache_sz = n + 1;

    return cache[n];
}
