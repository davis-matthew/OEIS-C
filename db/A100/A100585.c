/*
 * A100585  a(n+1) = a(n)+floor(a(n)/3), a(1) = 3.
 * https://oeis.org/A100585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3, a(n+1) = a(n) + floor(a(n)/3)
 * Range:     u8 n <= 17, u16 n <= 37, u32 n <= 75, u64 n <= 152,
 *            u128 n <= 306, big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms currently stored          */
static size_t cache_cap  = 0;   /* current capacity of the cache array        */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1. For n == 0 the caller
       may simply ignore the returned value. */
    if (n == 0) return (seq_t)0;

    /* Grow the cache until it contains a(n).  The growth is incremental
       but the helper function ensures the underlying array is resized
       efficiently. */
    while (cache_size < n)
    {
        if (cache_size == 0)
        {
            /* Initialise the first term a(1) = 3. */
            ensure_capacity((size_t)1, &cache, &cache_cap);
            cache[0] = (seq_t)3;
            cache_size = 1;
            continue;
        }

        /* Compute the next term from the recurrence. */
        seq_t prev = cache[cache_size - 1];
        seq_t add  = prev / (seq_t)3;
        seq_t next = prev + add;

        /* Make room for the new term. */
        ensure_capacity(cache_size + 1, &cache, &cache_cap);
        cache[cache_size] = next;
        cache_size++;
    }

    return cache[n - 1];
}
