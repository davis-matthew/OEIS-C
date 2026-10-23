/*
 * A145526  Numbers Y such that 273*Y^2+91 is a square.
 * https://oeis.org/A145526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 1454*a(n+1) – a(n); a(1)=11, a(2)=16005
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* a(0) is unused (offset = 1) */
    if (n < 1)
        return 0;

    /* Static cache to keep already computed terms */
    static seq_t *cache   = NULL;
    static idx_t  cache_size = 0;   /* number of allocated slots (including index 0) */
    static idx_t  max_computed = 0; /* highest n for which cache[n] is valid */

    /* Resize (or allocate) when necessary */
    if (cache == NULL || n + 1 > cache_size) {
        /* Allocate a fresh array that can hold indices 0 .. n */
        seq_t *new_cache = seq_alloc_array(n + 1);
        /* Ensure it is initialized; seq_alloc_array guarantees zero-init for integer types
          and default-constructs for user-defined types. */
        new_cache[0] = 0;          /* unused slot */
        new_cache[1] = 11;         /* a(1) */
        if (n >= 2)
            new_cache[2] = 16005; /* a(2) */

        /* Compute any remaining terms up to n */
        for (idx_t i = 3; i <= n; ++i)
            new_cache[i] = 1454 * new_cache[i - 1] - new_cache[i - 2];

        /* Dispose old cache if it existed */
        if (cache != NULL)
            free_seq_array(cache);

        cache        = new_cache;
        cache_size   = n + 1;
        max_computed = n;
        return cache[n];
    }

    /* Common case: cache has enough capacity */
    if (max_computed < n) {
        /* Extend the cache forward (no need to reallocate) */
        for (idx_t i = max_computed + 1; i <= n; ++i)
            cache[i] = 1454 * cache[i - 1] - cache[i - 2];
        max_computed = n;
    }

    return cache[n];
}
