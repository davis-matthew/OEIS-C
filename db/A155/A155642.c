/*
 * A155642  a(n) = 9^n - 5^n + 1.
 * https://oeis.org/A155642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - 5^n + 1. Recurrence: a(0) = 1, a(1) = 5, a(n) =
 *            14*a(n-1) - 45*a(n-2) + 32 for n ≥ 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Special‑case the two base values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* Static cache of computed values */
    static seq_t *cache = NULL;          /* array of a(i) */
    static size_t cache_len = 0;         /* current number of stored values */
    static size_t cache_cap = 0;         /* allocated capacity of the array */

    /* Helper: guarantee that the cache can hold 'need' elements. */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 4;
        while (new_cap < (size_t)n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* keep behaviour deterministic */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the cache on first use. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache_len = 2;
    }

    /* Grow the cache up to the requested index using the recurrence. */
    while (cache_len <= (size_t)n) {
        seq_t t1 = (seq_t)14 * cache[cache_len - 1];
        seq_t t2 = (seq_t)45 * cache[cache_len - 2];
        cache[cache_len] = t1 - t2 + (seq_t)32;
        ++cache_len;
    }

    return cache[n];
}
