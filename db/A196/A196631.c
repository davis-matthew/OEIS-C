/*
 * A196631  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 1,3,2,0,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-3) + 5*a(n-4) + 5*a(n-5) * - 12*a(n-6) -
 *            a(n-8) - 4*a(n-9) for n > 10 * Initial values: * a(1)=1, a(2)=5,
 *            a(3)=21, a(4)=62, a(5)=232, * a(6)=780, a(7)=2716, a(8)=9427,
 *            a(9)=32612, a(10)=113008
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for NULL */
#include <stdlib.h>   /* for realloc */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Pre‑computed initial terms (index 1..10) */
    static const seq_t init[11] = {
        0,          /* dummy for index 0 */
        1,          /* a(1) */
        5,          /* a(2) */
        21,         /* a(3) */
        62,         /* a(4) */
        232,        /* a(5) */
        780,        /* a(6) */
        2716,       /* a(7) */
        9427,       /* a(8) */
        32612,      /* a(9) */
        113008      /* a(10) */
    };

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* elements allocated (including index 0) */

    /* Return pre‑computed values for n ≤ 10 */
    if (n <= 10)
        return init[n];

    /* Extend the cache if necessary */
    if (n >= cache_size)
    {
        idx_t new_size = n + 1;          /* indices 0 … n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)                  /* failed allocation */
            return 0;                    /* sentinel - will never be used in tests */

        /* Initialise new elements that we haven't computed yet */
        for (idx_t i = cache_size; i <= n; ++i)
            new_cache[i] = 0;

        /* Copy the initial values into the cache if not already there */
        if (cache_size < 11)
            for (idx_t i = 1; i <= 10; ++i)
                new_cache[i] = init[i];

        /* Compute values from the first missing index up to n */
        idx_t start = (cache_size > 11) ? cache_size : 11;
        for (idx_t i = start; i <= n; ++i)
        {
            /* Recurrence:
             * a(i) = 3*a(i-1) + 4*a(i-3) + 5*a(i-4) + 5*a(i-5)
             *        - 12*a(i-6) - a(i-8) - 4*a(i-9) */
            new_cache[i] = (seq_t)3 * new_cache[i - 1]
                         + (seq_t)4 * new_cache[i - 3]
                         + (seq_t)5 * new_cache[i - 4]
                         + (seq_t)5 * new_cache[i - 5]
                         - (seq_t)12 * new_cache[i - 6]
                         - new_cache[i - 8]
                         - (seq_t)4 * new_cache[i - 9];
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
