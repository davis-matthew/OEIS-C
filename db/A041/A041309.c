/*
 * A041309  Denominators of continued fraction convergents to sqrt(167).
 * https://oeis.org/A041309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 336*a(n-4) - a(n-8) with initial values: a(0)=1, a(1)=1,
 *            a(2)=12, a(3)=13, a(4)=324, a(5)=337, a(6)=4031, a(7)=4368
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initial 8 terms hard‑coded; they fit in any seq_t */
    const seq_t init[8] = { (seq_t)1,  (seq_t)1,  (seq_t)12, (seq_t)13,
                            (seq_t)324, (seq_t)337, (seq_t)4031, (seq_t)4368 };

    /* Initialize cache on first call */
    if (cache_len == 0) {
        cache_cap = 16;                 /* start with room for >8 terms */
        cache = seq_realloc(cache, 0, cache_cap);
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        cache_len = 8;
    }

    /* Extend cache up to the requested index if needed */
    while (n >= cache_len) {
        /* Grow capacity if necessary */
        if ((size_t)cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap <= (size_t)cache_len + 1)
                new_cap = cache_len + 1;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }

        /* Compute next term using the recurrence */
        cache[cache_len] = (seq_t)336 * cache[cache_len - 4] - cache[cache_len - 8];
        ++cache_len;
    }

    return cache[n];
}
