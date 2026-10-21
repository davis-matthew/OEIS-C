/*
 * A096975  Trace sequence of a path graph plus loop.
 * https://oeis.org/A096975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3); initial terms: a(0)=3,
 *            a(1)=1, a(2)=5 Uses a simple linear‑recurrence with caching
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 75, u128 n <= 150,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of computed terms.  cache[i] holds a(i).
   cache_size == number of allocated elements (next free index). */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Constant 2 in type seq_t */
static const seq_t TWO = (seq_t)2;

seq_t a(idx_t n)
{
    /* Handle the first three indices directly (no allocation needed). */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    /* Ensure the first three terms are cached. */
    if (cache_size == 0) {
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        if (!cache) abort();          /* allocation failed */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache_size = 3;
    }

    /* If already computed, return it. */
    if (n < cache_size) {
        return cache[n];
    }

    /* Extend the cache to hold all terms up to n. */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();               /* allocation failed */
    cache = tmp;

    /* Compute all new terms using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i-1] + TWO * cache[i-2] - cache[i-3];
    }
    cache_size = new_size;
    return cache[n];
}
