/*
 * A041834  Numerators of continued fraction convergents to sqrt(438).
 * https://oeis.org/A041834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041834: a(n) = 586*a(n-4) - a(n-8) with
   a(0)=20, a(1)=21, a(2)=272, a(3)=293,
   a(4)=11992, a(5)=12285, a(6)=159412, a(7)=171697.
   The function memoises results so it works in any call order. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_size = 0;  /* number of already computed terms */

    size_t target = (size_t)n;          /* target index to return */

    /* initialise cache on first call ---------------------------------- */
    if (!cache) {
        /* start with space for at least 8 elements (the recurrence
           needs a(8) when computing a(8)) */
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory – fall back to 0 */

        /* initialise first 8 terms */
        static const seq_t init[8] = {
            (seq_t)20,    /* a(0) */
            (seq_t)21,    /* a(1) */
            (seq_t)272,   /* a(2) */
            (seq_t)293,   /* a(3) */
            (seq_t)11992, /* a(4) */
            (seq_t)12285, /* a(5) */
            (seq_t)159412,/* a(6) */
            (seq_t)171697 /* a(7) */
        };
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        cache_size = 8;
    }

    /* extend cache if we need a bigger index -------------------------- */
    if (target >= cache_size) {
        /* ensure capacity for target+1 elements */
        if (target + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < target + 1)
                new_cap = target + 1;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) return 0;   /* out of memory – return 0 */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* compute missing terms up to target */
        for (size_t i = cache_size; i <= target; ++i) {
            /* regular unsigned arithmetic – overflow wraps */
            cache[i] = cache[i - 4] * (seq_t)586 - cache[i - 8];
        }
        cache_size = target + 1;
    }

    return cache[target];
}
