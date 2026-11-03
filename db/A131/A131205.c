/*
 * A131205  a(n) = a(n-1) + a(floor(n/2)) + a(ceiling(n/2)).
 * https://oeis.org/A131205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n‑1) + a(floor(n/2)) + a(ceil(n/2)) with a(1) = 1
 *            (offset 1)
 * Range:     u8 n <= 11, u16 n <= 45, u32 n <= 310, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 928 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    /* Cache of computed values. cache[i] holds a(i) for i ≥ 1.
       cache[0] is unused but kept to simplify indexing. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size in elements */
    static idx_t  cache_len = 0;   /* highest index with a computed value */

    /* n must be ≥ 1 */
    if (n == 0)
        return (seq_t)0;   /* undefined, but avoid UB */

    /* Initialise the cache on first use. */
    if (cache == NULL)
    {
        cache = alloc_seq(2);                /* indices 0 and 1 */
        if (cache == NULL)
            return (seq_t)0;                 /* allocation failed */
        cache[0] = (seq_t)0;                /* unused */
        cache[1] = (seq_t)1;                /* a(1) = 1 */
        cache_cap = 2;
        cache_len = 1;
    }

    /* Grow the cache if we need a larger index. */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;
        grow_seq_array(&cache, &cache_cap, new_cap);
        /* Newly allocated elements are default‑constructed (zero). */
    }

    /* Compute missing terms iteratively up to n. */
    if ((idx_t)n > cache_len)
    {
        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            idx_t f = i >> 1;           /* floor(i/2) */
            idx_t c = (i + 1) >> 1;     /* ceil(i/2) */
            cache[i] = cache[i - 1] + cache[f] + cache[c];
        }
        cache_len = n;
    }

    return cache[n];
}
