/*
 * A037615  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,2.
 * https://oeis.org/A037615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = value of the first n digits of the repeating base-8 *
 *            sequence 1,3,2,1,3,2,…, interpreted as an octal number
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = a(n–1)*8 + d(n),   with d(n) = 1,3,2 repeat;  a(1)=1. */

#include <stddef.h>
#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n)
{
    /* Offset starts at 1; ignore requests for n==0 (undefined). */
    if (n == 0) return 0;

    static seq_t *cache = NULL;   /* cache[i] = a(i), 0 ≤ i < cap */
    static size_t  cap  = 0;      /* size of allocated array */

    /* Grow cache if needed */
    if ((size_t)n >= cap) {
        size_t newcap = (size_t)n + 1;      /* store indices 0…n */
        seq_t *newcache = alloc_cache(newcap);

        /* Copy existing values */
        if (cache != NULL) {
            for (size_t i = 0; i < cap; ++i)
                newcache[i] = cache[i];
        } else {
            newcache[0] = 0;                 /* dummy entry at 0 */
        }

        /* Compute new values starting from the first missing index */
        size_t start = (cap == 0) ? 1 : cap;
        for (size_t i = start; i < newcap; ++i) {
            unsigned digit;
            size_t mod = (i - 1) % 3;       /* 0→1, 1→3, 2→2 */
            if (mod == 0) digit = 1;
            else if (mod == 1) digit = 3;
            else             digit = 2;
            newcache[i] = newcache[i - 1] * (seq_t)8 + (seq_t)digit;
        }

        /* Free old cache and update pointers */
        if (cache != NULL) free_cache(cache);
        cache = newcache;
        cap   = newcap;
    }

    return cache[n];
}
