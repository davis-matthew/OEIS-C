/*
 * A042131  Denominators of continued fraction convergents to sqrt(590).
 * https://oeis.org/A042131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 11562*a(n‑6) – a(n‑12) (n > 11) Initial values: a(0)=1,
 *            a(1)=3, a(2)=7, a(3)=31, a(4)=69, a(5)=238, a(6)=11493,
 *            a(7)=34717, a(8)=80927, a(9)=358425, a(10)=797777, a(11)=2751756
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;   /* pointer to cached terms */
    static size_t cache_cap = 0;  /* current capacity (number of slots) */

    /* Ensure cache has enough space for indices 0 … n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : ((size_t)n + 12);
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure → zero (safe fallback) */

        /* Zero any new slots that were not previously allocated */
        if (new_cap > cache_cap) {
            memset((void *)(tmp + cache_cap), 0,
                   (new_cap - cache_cap) * sizeof(seq_t));
        }

        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first 12 terms if they are still zero */
    static const seq_t init[12] = {
        (seq_t)1,  (seq_t)3,  (seq_t)7,  (seq_t)31,
        (seq_t)69, (seq_t)238, (seq_t)11493, (seq_t)34717,
        (seq_t)80927, (seq_t)358425, (seq_t)797777, (seq_t)2751756
    };
    for (size_t i = 0; i < 12 && i < cache_cap; ++i) {
        if (!cache[i]) cache[i] = init[i];
    }

    /* Compute missing terms up to index n using the recurrence */
    for (size_t i = 12; i <= (size_t)n; ++i) {
        if (cache[i]) continue; /* already computed in a previous call */

        /* Use seq_t arithmetic; overflow on unsigned types is acceptable
           once the value no longer fits in the type. */
        cache[i] = cache[i - 6] * (seq_t)11562 - cache[i - 12];
    }

    return cache[(size_t)n];
}
