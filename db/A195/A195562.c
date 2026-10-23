/*
 * A195562  Denominators a(n) of Pythagorean approximations b(n)/a(n) to 1/4.
 * https://oeis.org/A195562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 65*a(n-3) + 65*a(n-6) - a(n-9) Initial values: a(1)=1,
 *            a(2)=24, a(3)=40, a(4)=63, a(5)=1600, a(6)=2624, a(7)=4161,
 *            a(8)=105560, a(9)=173160. The function memoizes the values so it
 *            works in any call order
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Forward declaration of the function to be implemented */
seq_t a(idx_t n);

/* Private helper: grow the cache to at least 'needed' elements */
static void _ensure_cache(idx_t needed)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;

    if (!cache) {
        /* allocate first 9 slots for the initial terms */
        cap = 9;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return; /* allocation failure: silently fail, caller will crash */
        static const seq_t init[9] = {1, 24, 40, 63, 1600, 2624, 4161, 105560, 173160};
        for (size_t i = 0; i < 9; ++i) cache[i] = init[i];
    }

    if (needed <= cap) return;

    size_t new_cap = cap;
    while (new_cap < (size_t)needed) new_cap = new_cap * 2 + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) return; /* allocation failure */
    cache = new_cache;
    cap = new_cap;
}

/* Exported function as required by the problem */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n=0, but safe guard */

    /* Memoization using a static cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (!cache) {
        /* initialise cache with first 9 values */
        cache_size = 9;
        cache = (seq_t *)malloc(9 * sizeof(seq_t));
        static const seq_t init[9] = {1, 24, 40, 63, 1600, 2624, 4161, 105560, 173160};
        for (size_t i = 0; i < 9; ++i) cache[i] = init[i];
    }

    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* grow cache to hold up to n values */
    if (cache_size < (size_t)n) {
        size_t old_cap = (size_t)cache_size == 0 ? 9 : (2 * cache_size + 1);
        while (old_cap < (size_t)n) old_cap = old_cap * 2 + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, old_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure, return 0 */
        cache = new_cache;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t idx = cache_size; idx < n; ++idx) {
        /* idx is 0-based index of a(idx+1) */
        seq_t term1 = (seq_t)65 * cache[idx - 3];
        seq_t term2 = (seq_t)65 * cache[idx - 6];
        seq_t cur   = term1 + term2 - cache[idx - 9];
        cache[idx] = cur;
    }

    cache_size = n;
    return cache[n - 1];
}
