/*
 * A220361  a(n) = Fibonacci(n)^3 + (-1)^n*Fibonacci(n-2).
 * https://oeis.org/A220361
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)+6*a(n-2)-3*a(n-3)-a(n-4); Offset 2: a(2)=1,
 *            a(3)=7, a(4)=28, a(5)=123
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2. For n < 2 the value is
       irrelevant; we simply return 0. */
    if (n < 2) return 0;

    /* Static cache for memoisation of previously computed values. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    /* How far the cache has been filled.  Initially 1 (no valid entry). */
    static idx_t  computed_upto = 1;

    /* Ensure the array can hold indices up to n. */
    ensure_cache(n, &cache, &cap);

    /* Initialise the first four known values if they have not been set
       yet.  They are always present after the first expansion. */
    if (computed_upto < 5) {
        cache[2] = 1;
        cache[3] = 7;
        cache[4] = 28;
        cache[5] = 123;
        computed_upto = 5;
    }

    /* Compute new values up to n only once per value.  Subsequent
       calls with the same or smaller n simply return the cached
       number. */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        /* a(i) = 3*a(i-1) + 6*a(i-2) - 3*a(i-3) - a(i-4) */
        cache[i] = 3 * cache[i-1] + 6 * cache[i-2]
                   - 3 * cache[i-3] - cache[i-4];
    }
    computed_upto = n;

    return cache[n];
}
