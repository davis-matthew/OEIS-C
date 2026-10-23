/*
 * A120446  Expansion of 1/(1-x-x^4-x^6).
 * https://oeis.org/A120446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 60, u64 n <= 118,
 *            u128 n <= 234, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-4) + a(n-6);
   initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=2, a(5)=3, a(6)=5 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Static cache of computed values. */
    static seq_t *cache = NULL;   /* points to array of seq_t */
    static size_t cap = 0;        /* number of allocated slots */
    static idx_t last = (idx_t)-1;        /* highest index computed */

    /* Initialize base values on the first call. */
    if (last == (idx_t)-1) {
        /* Allocate space for indices 0..6. */
        ensure_cache(7, &cache, &cap);
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 2;
        cache[5] = 3;
        cache[6] = 5;
        last = 6;
    }

    /* Return already computed value. */
    if (n <= last)
        return cache[n];

    /* Ensure enough space for indices up to n. */
    ensure_cache(n + 1, &cache, &cap);

    /* Compute new values from the last computed index up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* For i >= 7, the indices i-1, i-4, and i-6 are valid. */
        cache[i] = cache[i - 1] + cache[i - 4] + cache[i - 6];
    }
    last = n;
    return cache[n];
}
