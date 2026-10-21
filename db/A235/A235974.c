/*
 * A235974  Number of (n+1) X (1+1) 0..2 arrays with the upper median equal to
 *          the lower median in every 2 X 2 subblock.
 * https://oeis.org/A235974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*a(n-1) + 7*a(n-2) - 22*a(n-3) + 8*a(n-4)
 * Initial terms: a(1)=39, a(2)=181, a(3)=815, a(4)=3741
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n < 1 */

    /* base values for indices 1..4 */
    static const seq_t base[5] = {0, 39, 181, 815, 3741};

    if (n < 5)
        return base[n];

    /* dynamic cache for already computed terms (1‑based) */
    static seq_t *cache = NULL;   /* array of seq_t */
    static idx_t cache_cap = 0;   /* number of allocated slots (including index 0) */
    static idx_t cached_max = 0;  /* largest index that has been computed */

    /* allocate or grow the cache as needed */
    if (cache == NULL) {
        cache_cap = n + 1;                 /* need indices 0 .. n */
        cache = (seq_t *)calloc((size_t)cache_cap, sizeof(seq_t));
        if (!cache)
            return 0;                      /* allocation failure */
        /* store base terms */
        cache[1] = base[1];
        cache[2] = base[2];
        cache[3] = base[3];
        cache[4] = base[4];
        cached_max = 4;
    } else if (n + 1 > cache_cap) {
        /* enlarge the cache */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp)
            return 0;                      /* allocation failure */
        /* initialise the new slots to zero */
        for (idx_t i = cache_cap; i < n + 1; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = n + 1;
    }

    /* compute any missing terms up to n */
    if (cached_max < n) {
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            cache[i] = (seq_t)4 * cache[i-1]
                     + (seq_t)7 * cache[i-2]
                     - (seq_t)22 * cache[i-3]
                     + (seq_t)8 * cache[i-4];
        }
        cached_max = n;
    }

    return cache[n];
}
