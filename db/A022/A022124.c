/*
 * A022124  Fibonacci sequence beginning 3, 13.
 * https://oeis.org/A022124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3, a(1)=13, a(n)=a(n-1)+a(n-2) for n≥2
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoization cache for seq_t values. */
static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_cap = 0;         /* allocated capacity (in elements) */
static size_t cache_size = 0;        /* number of initialized elements */

/* Ensure the cache can store at least indices 0 … n (inclusive). */
static void grow_to(idx_t n)
{
    /* Number of elements required. */
    size_t needed = (size_t)(n + 1UL);
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2ULL : 2ULL;
        if (new_cap < needed)
            new_cap = needed;
        /* Cast result of realloc for C++ compatibility. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Sequence A022124: Fibonacci sequence starting with 3, 13. */
seq_t a(idx_t n)
{
    /* Make sure the cache can hold index n. */
    grow_to(n);

    /* Initialise the first two terms if not already done. */
    if (cache_size < 2) {
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)13;
        cache_size = 2;
    }

    /* Compute any missing terms up to n. */
    if (cache_size <= n) {
        for (idx_t i = (idx_t)cache_size; i <= n; ++i)
            cache[i] = cache[i - 1] + cache[i - 2];
        cache_size = (size_t)(n + 1UL);
    }

    return cache[n];
}
