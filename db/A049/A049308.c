/*
 * A049308  Sextuple factorial numbers: Product_{k=0..n-1} (6*k+4).
 * https://oeis.org/A049308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(n)=2*(3*n-1)*a(n-1) (equivalently a(n)=(6*n-2)*a(n-1))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*   The function can be called in any order; a static cache memoises
 *   all already computed values.  The cache is grown as needed.   */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of computed a(i) */
    static idx_t  cached_max = 0;   /* highest index computed (inclusive) */
    static size_t cached_cap = 0;   /* current capacity of cache array */

    /* initialise cache with a(0)=1 */
    if (!cache)
    {
        cached_cap = 1;
        cache = (seq_t *)malloc(cached_cap * sizeof(seq_t));
        if (!cache) abort();         /* out of memory */
        cache[0] = (seq_t)1;
        cached_max = 0;
    }

    /* extend cache up to requested n */
    while (cached_max < n)
    {
        /* next index to compute */
        idx_t i = cached_max + 1;

        /* ensure sufficient capacity */
        if ((size_t)i >= cached_cap)
        {
            size_t new_cap = cached_cap ? cached_cap * 2 : 4;
            while ((size_t)i >= new_cap) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cached_cap = new_cap;
        }

        /* factor = 6*i - 2 , computed as seq_t */
        seq_t factor = (seq_t)(6 * i - 2);
        cache[i] = cache[cached_max] * factor;   /* recurrence */

        cached_max = i;
    }

    return cache[n];
}
