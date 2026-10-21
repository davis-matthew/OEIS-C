/*
 * A209603  Number of n X 1 1..2 arrays with every element value z a city
 *          block distance of exactly z from another element value z.
 * https://oeis.org/A209603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-6) for n > 6 with a(1) = 0, a(2)
 *            = 1, a(3) = 1, a(4) = 2, a(5) = 2, a(6) = 4
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 60, u64 n <= 118,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values. */
    static seq_t *cache = NULL;   /* cached values, cache[i] holds a(i+1) */
    static idx_t cached = 0;      /* number of values currently cached */
    static idx_t capacity = 0;    /* allocated capacity of cache */

    /* Grow the cache if necessary. */
    if (n > capacity) {
        size_t new_cap = (capacity == 0) ? n : capacity * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        capacity = (idx_t)new_cap;
    }

    /* Compute values that are not yet cached. */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)           val = 0;
        else if (i == 2)      val = 1;
        else if (i == 3)      val = 1;
        else if (i == 4)      val = 2;
        else if (i == 5)      val = 2;
        else if (i == 6)      val = 4;
        else
            val = (seq_t)2 * cache[i - 2] - cache[i - 3] + cache[i - 7];
        cache[i - 1] = val;
    }

    cached = n;
    return cache[n - 1];
}
