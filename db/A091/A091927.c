/*
 * A091927  Expansion of (1-6*x)/(1-6*x-5*x^2).
 * https://oeis.org/A091927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) + 5*a(n-2), a(0)=1, a(1)=0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper to grow a pointer array */
static void *grow(void *ptr, size_t *cap, size_t need, size_t elem_size)
{
    if (*cap < need) {
        size_t new_cap = *cap ? *cap : 4;
        while (new_cap < need) new_cap <<= 1;
        ptr = realloc(ptr, new_cap * elem_size);
        *cap = new_cap;
    }
    return ptr;
}

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;   /* number of initialized terms          */
    static idx_t cache_cap = 0;   /* allocated capacity (in terms)         */

    /* Initialize base values once  */
    if (!cache) {
        cache_cap = 4;
        cache_len = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)0;   /* a(1) */
    }

    /* Extend cache up to requested n if necessary */
    if (cache_len <= n) {
        cache = (seq_t *)grow((void *)cache, &cache_cap, n + 1, sizeof(seq_t));
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence: a(i) = 6*a(i-1) + 5*a(i-2)  (holds for i>=2) */
            seq_t t = cache[i - 1] * 6 + cache[i - 2] * 5;
            cache[i] = t;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
