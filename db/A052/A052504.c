/*
 * A052504  Number of permutations sigma of [5n] without fixed points such
 *          that sigma^5 = Id.
 * https://oeis.org/A052504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5*n)! / (n! * 5^n); Recurrence: a(0) = 1, a(n) = a(n-1)
 *            * (5*n-1) * (5*n-2) * (5*n-3) * (5*n-4) for n >= 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache   = NULL;   /* dynamically allocated array of seq_t */
    static size_t        cached = 0;  /* number of terms currently stored (cache[0] ... cache[cached-1]) */
    static size_t        cap    = 0;  /* capacity of the allocated array */

    /* initialise cache with a(0) = 1 */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* out of memory: return 0 as a sentinel */
        cache[0] = (seq_t)1;
        cached = 1;
    }

    /* If the requested term is already in the cache, return it */
    if ((size_t)n < cached) {
        return cache[(size_t)n];
    }

    /* Extend the cache up to index n */
    for (size_t cur = cached; cur <= (size_t)n; ++cur) {
        /* Ensure there is enough space; double capacity when needed */
        if (cur >= cap) {
            size_t new_cap = cap * 2;
            if (new_cap < cur + 1) new_cap = cur + 1;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (new_cache == NULL) return (seq_t)0;  /* out of memory: return sentinel */
            cache = new_cache;
            cap   = new_cap;
        }

        /* Compute factor = (5*cur-1)*(5*cur-2)*(5*cur-3)*(5*cur-4) */
        seq_t f = (seq_t)(5 * cur - 1);
        f *= (seq_t)(5 * cur - 2);
        f *= (seq_t)(5 * cur - 3);
        f *= (seq_t)(5 * cur - 4);

        /* a(cur) = a(cur-1) * f */
        cache[cur] = cache[cur - 1] * f;
    }

    cached = (size_t)n + 1;
    return cache[(size_t)n];
}
