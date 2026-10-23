/*
 * A021686  Decimal expansion of 1/682.
 * https://oeis.org/A021686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal digits of 1/682
   a(n) = the (n+1)th digit after the decimal point in the expansion
   of 1/682.  Computed by long‑division. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of already computed digits */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of digits stored (cache_len = highest_index + 1) */
    static size_t cache_cap = 0;   /* allocated capacity of the cache */
    static seq_t rem = 1;          /* current remainder after the last computed digit */

    /* Need at least n+1 digits (indices 0 … n) */
    size_t target = (size_t)(n + 1);

    /* Grow cache if necessary */
    if (target > cache_cap) {
        seq_t *new_cache = alloc_seq_array(target);
        if (cache) {
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
        }
        cache = new_cache;
        cache_cap = target;
    }

    /* Generate missing digits */
    for (size_t i = cache_len; i < target; ++i) {
        seq_t digit = (rem * 10) / (seq_t)682;      /* integer division yields the next digit */
        rem = (rem * 10) % (seq_t)682;              /* updated remainder */
        cache[i] = digit;
    }

    cache_len = target;
    return cache[n];
}
