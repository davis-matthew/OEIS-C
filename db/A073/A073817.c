/*
 * A073817  Tetranacci numbers with different initial conditions: a(n) =
 *          a(n-1) + a(n-2) + a(n-3) + a(n-4) starting with a(0)=4, a(1)=1,
 *          a(2)=3, a(3)=7.
 * https://oeis.org/A073817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=4, a(1)=1, a(2)=3, a(3)=7, a(n)=a(n-1)+a(n-2)+a(n-3)+a(n-4)
 *            for n>=4
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 67, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* initial terms (indexed 0..3) */
    static const seq_t init[4] = {
        (seq_t)4,
        (seq_t)1,
        (seq_t)3,
        (seq_t)7
    };

    /* cache of previously computed values */
    static seq_t *cache      = NULL;
    static idx_t  cache_cap  = 0;   /* number of elements allocated (>= last_cached+1) */
    static idx_t  last_cached = 3;  /* highest index for which cache[i] is defined */

    /* handle tiny indices directly */
    if (n < 4)
        return init[n];

    /* allocate and initialise cache on first use */
    if (cache == NULL)
    {
        cache_cap = 4;                      /* store indices 0..3 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)           /* allocation failed – return zero */
            return (seq_t)0;
        memcpy(cache, init, sizeof(init));
        last_cached = 3;
    }

    /* extend cache if requested index is beyond current range */
    if (n > last_cached)
    {
        /* enlarge capacity to at least (n+1) */
        if (n + 1 > cache_cap)
        {
            idx_t new_cap = cache_cap;
            while (new_cap <= n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp)          /* reallocation failed – return zero */
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = last_cached + 1; i <= n; ++i)
        {
            cache[i] = cache[i - 1] + cache[i - 2]
                       + cache[i - 3] + cache[i - 4];
        }
        last_cached = n;
    }

    return cache[n];
}
