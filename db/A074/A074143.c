/*
 * A074143  a(1) = 1; a(n) = n * Sum_{k=1..n-1} a(k).
 * https://oeis.org/A074143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{k=1}^{n-1} a(k) (a(1) = 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterative memoization using a cumulative sum; works for any build
   that defines seq_t and idx_t as described. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static state for memoization */
    static seq_t  *cache      = NULL;   /* cache[i] holds a(i+1) */
    static idx_t   max_cached = 0;      /* highest n computed so far */
    static size_t  capacity   = 0;      /* allocated size of cache    */
    static seq_t   sum_so_far = 0;      /* Σ_{k=1}^{max_cached} a(k) */

    /* Initialisation for n = 1 */
    if (max_cached == 0) {
        capacity   = 1;
#ifdef __cplusplus
        cache = new seq_t[capacity];
#else
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
#endif
        if (!cache) return (seq_t)0;          /* allocation failure → 0 */
        cache[0]   = (seq_t)1;                /* a(1) = 1 */
        sum_so_far = (seq_t)1;                /* cumulative sum after a(1) */
        max_cached = 1;
    }

    /* Return cached value if already computed */
    if (n <= max_cached)
        return cache[n - 1];

    /* Grow the cache if necessary */
    if ((size_t)n > capacity) {
        size_t new_cap = (size_t)n;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        /* Copy existing elements */
        for (idx_t i = 0; i < max_cached; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
#else
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* realloc preserves existing contents in C */
#endif
        capacity = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t term = (seq_t)i * sum_so_far;
        cache[i - 1] = term;
        sum_so_far += term;
    }

    max_cached = n;
    return cache[n - 1];
}
