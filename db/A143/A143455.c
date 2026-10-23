/*
 * A143455  Expansion of 1/(x^k*(1-x-3*x^(k+1))) for k=4.
 * https://oeis.org/A143455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 49, u64 n <= 101,
 *            u128 n <= 204, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A143455 – Expansion of 1/(x^4*(1-x-3*x^5))
   Recurrence: a(0)=1, a(1)=4, a(2)=7, a(3)=10, a(4)=13
                a(n)=a(n-1)+3*a(n-5)   for n≥5
   This implementation caches computed terms so that arbitrary
   call order is handled in O(1) after the first evaluation of a value. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached values */
    static size_t cache_size = 0;    /* number of elements currently cached */

    /* Guard against indices that would overflow a C array index. */
    if (n > (idx_t)(SIZE_MAX - 1))
        abort();

    size_t idx = (size_t)n;          /* index into the cache */

    /* Expand cache if necessary. */
    if (idx + 1 > cache_size) {
        seq_t *new_cache = alloc_seq_array(idx + 1); /* provided helper */
        if (!new_cache)
            abort();

        /* copy previously computed elements */
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
        }

        /* compute new entries (including the first ones) */
        for (size_t i = cache_size; i <= idx; ++i) {
            switch (i) {
                case 0: new_cache[0] = (seq_t)1; break;
                case 1: new_cache[1] = (seq_t)4; break;
                case 2: new_cache[2] = (seq_t)7; break;
                case 3: new_cache[3] = (seq_t)10; break;
                case 4: new_cache[4] = (seq_t)13; break;
                default:
                    new_cache[i] = new_cache[i-1] + ((seq_t)3 * new_cache[i-5]);
                    break;
            }
        }

        /* replace old cache */
        if (cache)
            free(cache);
        cache = new_cache;
        cache_size = idx + 1;
    }

    return cache[idx];
}
