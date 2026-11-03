/*
 * A157725  a(n) = Fibonacci(n) + 2.
 * https://oeis.org/A157725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-1) - 2 (for n ≥ 2), with a(0) = 2, a(1) = 3
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Validated: standard, pass, 57 terms through n = 262 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements Fibonacci(n) + 2 via the recurrence relation. */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache of computed a(n) values */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;

    /* prime the cache for a(0) and a(1) */
    if (cache_max == 0) {
        cache = mem_new(seq_t, 2);
        if (!cache) abort();          /* allocation failure */
        cache[0] = (seq_t)2;          /* a(0) */
        cache[1] = (seq_t)3;          /* a(1) */
        cache_max = 2;
    }

    /* already computed? */
    if (n < cache_max)
        return cache[n];

    /* extend the cache to include a(n) */
    idx_t new_size = n + 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_size);
    if (!tmp) abort();              /* allocation failure */
    cache = tmp;

    /* compute missing values using the recurrence */
    for (idx_t i = cache_max; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-2) - 2  (mod seq_t width) */
        cache[i] = cache[i-1] + cache[i-2] - (seq_t)2;
    }
    cache_max = new_size;

    return cache[n];
}
