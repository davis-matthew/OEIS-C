/*
 * A023384  a(n+1) = a(n) converted to base 7 from base 6 (written in base
 *          10).
 * https://oeis.org/A023384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n) converted from base‑6 to base‑7 (in base‑10); a(1)
 *            = 6 (OEIS A023384)
 * Range:     u8 n <= 21, u16 n <= 31, u32 n <= 40, u64 n <= 49, u128 n <= 58,
 *            big any n
 * Helpers:   conv_base6_to_base7, seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"
#include "../utils/conv_base6_to_base7.h"

seq_t a(idx_t n)
{
    /* Cache the sequence values so that repeated calls are fast and order
       of calls does not matter.  The cache is allocated lazily. */
    static seq_t *cache    = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored */
    static size_t cache_cap = 0;   /* allocated capacity (in terms) */

    if (n == 0)          /* behaviour for n < 1 is undefined by the OEIS
                            offset; returning 0 is harmless. */
        return 0;

    /* Initialise the cache with the first term a(1) = 6. */
    if (cache == NULL) {
        cache_cap = 1;
        cache = seq_alloc_new_array(cache_cap);
        cache[0] = 6;  /* a(1) = 6 */
        cache_len = 1;
    }

    /* Extend the cache up to the requested index n. */
    while (cache_len < (size_t)n) {
        seq_t next = conv_base6_to_base7(cache[cache_len - 1]);

        /* Grow the array if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            seq_t *new_cache = seq_alloc_new_array(new_cap);
            /* Copy existing values. */
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            /* Free the old array (all seq_t objects are destructed by seq_free). */
            seq_free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }

        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[n - 1];
}
