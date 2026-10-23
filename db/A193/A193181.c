/*
 * A193181  a(n) = lcm(f(1),f(2),...,f(n)) with f(x) = x^2+1.
 * https://oeis.org/A193181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(f(1), f(2), ..., f(n)) where f(x) = x^2 + 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 27,
 *            big any n
 * Helpers:   alloc_seq_array, ensure_capacity, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* Static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;    /* Highest index computed so far */
    static size_t cap   = 0;      /* Allocated capacity of cache array */

    /* If a value for the requested index already exists, return it immediately */
    if (cached_n >= n) {
        if (cache == NULL) return 0;  /* should not happen */
        return cache[n];
    }

    /* Ensure the cache can hold up to index n (array is 1‑based; ignore cache[0]) */
    size_t needed = n + 1;  /* because we index from 1 to n inclusive */
    if (cap < needed) {
        ensure_capacity(needed, &cap, (void **)&cache, sizeof(seq_t));
    }

    /* Initialize the cache if it's the first call */
    if (cached_n == 0) {
        /* cache[1] = f(1) = 1^2 + 1 = 2 */
        cache[1] = (seq_t)2;
        cached_n = 1;
    }

    /* Compute lcm values from cached_n + 1 up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        /* term = f(i) = i^2 + 1 */
        seq_t ii = (seq_t)i * (seq_t)i;
        seq_t term = ii + 1;

        /* lcm(prev, term) = prev / gcd(prev, term) * term */
        seq_t g = gcd(cache[i - 1], term);
        seq_t temp = cache[i - 1] / g;
        cache[i] = temp * term;
    }

    cached_n = n;
    return cache[n];
}
