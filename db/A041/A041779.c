/*
 * A041779  Denominators of continued fraction convergents to sqrt(410).
 * https://oeis.org/A041779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 162*a(n-2)-a(n-4); Initial values: a(0)=1, a(1)=4,
 *            a(2)=161, a(3)=648
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cached_up_to = 0;

    /* Initialise cache with first four terms */
    if (cache == NULL) {
        cache_cap = 4;
        cache = alloc_seq(cache_cap);
        cache[0] = 1;
        cache[1] = 4;
        cache[2] = 161;
        cache[3] = 648;
        cached_up_to = 3;
    }

    /* Return already computed value */
    if (n <= cached_up_to)
        return cache[n];

    /* Grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;          /* double until big enough */
        seq_t *new_cache = alloc_seq(new_cap);
        for (idx_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        free_seq(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        cache[i] = ((seq_t)162) * cache[i - 2] - cache[i - 4];
    }
    cached_up_to = n;
    return cache[n];
}
