/*
 * A092492  Arises in enumeration of 321-hexagon-avoiding permutations.
 * https://oeis.org/A092492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 9*a(n-3) - 4*a(n-4) - 4*a(n-5) +
 *            a(n-6) for n > 7, with initial values a(1)=0, a(2)=0, a(3)=0,
 *            a(4)=0, a(5)=0, a(6)=1, a(7)=5
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 23, u64 n <= 41, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[8] =
{
    0,  /* dummy for index 0 */
    0,  /* a(1) */
    0,  /* a(2) */
    0,  /* a(3) */
    0,  /* a(4) */
    0,  /* a(5) */
    1,  /* a(6) */
    5   /* a(7) */
};

seq_t a(idx_t n)
{
    /* cache of computed values, indices 0..last_computed */
    static seq_t *cache = NULL;
    static idx_t last_computed = 0;      /* highest index that has been computed */
    static size_t cache_size = 0;        /* allocated size of cache (>= last_computed+1) */

    /* If n is within the known initial segment, return directly. */
    if (n <= 7)
        return base_vals[n];

    /* On first call (or when cache is empty and we need >7),
       allocate cache up to at least index 7. */
    if (cache == NULL)
    {
        size_t needed = 8;            /* we need indices 0..7 */
        if (n + 1 > needed)
            needed = (size_t)n + 1;   /* will also enlarge for the requested n */

        cache = (seq_t *)calloc(needed, sizeof(seq_t));
        if (!cache)
            return 0;                 /* out‑of‑memory → return 0 */

        /* initialise the base values in the cache */
        cache[1] = 0; cache[2] = 0; cache[3] = 0;
        cache[4] = 0; cache[5] = 0; cache[6] = 1;
        cache[7] = 5;
        cache_size = needed;
        last_computed = 7;
    }
    else if ((size_t)(n + 1) > cache_size)
    {
        /* enlarge the cache to accommodate index n */
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0;                 /* out‑of‑memory → return 0 */
        cache = tmp;
        cache_size = new_size;
    }

    /* If we need values beyond what we have already computed, compute them. */
    if (n > last_computed)
    {
        idx_t i;
        for (i = last_computed + 1; i <= n; ++i)
        {
            /* compute a(i) using the recurrence:
               a(i) = 6*a(i-1) - 11*a(i-2) + 9*a(i-3) - 4*a(i-4) - 4*a(i-5) + a(i-6)
               All terms are non‑negative; we perform additions first then a single subtraction
               to avoid unsigned underflow. */
            seq_t pos = 6 * cache[i - 1] + 9 * cache[i - 3] + cache[i - 6];
            seq_t neg = 11 * cache[i - 2] + 4 * cache[i - 4] + 4 * cache[i - 5];
            cache[i] = pos - neg;
        }
        last_computed = n;
    }

    return cache[n];
}
