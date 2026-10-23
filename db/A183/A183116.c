/*
 * A183116  Magnetic Tower of Hanoi, total number of moves, optimally solving
 *          the [RED ; NEUTRAL ; NEUTRAL] or [NEUTRAL ; NEUTRAL ; BLUE]
 *          pre-colored puzzle.
 * https://oeis.org/A183116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0,a(1)=1,a(2)=4,a(3)=11,a(4)=30 For n>=5: a(n) = 4*a(n-1) -
 *            2*a(n-2) - 2*a(n-3) - 5*a(n-4) + 6*a(n-5)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for arbitrary ordering of calls */
    static seq_t *cache   = NULL;
    static size_t  cache_size = 0;
    static const seq_t init[5] = { (seq_t)0, (seq_t)1, (seq_t)4, (seq_t)11, (seq_t)30 };

    /* If n is one of the initial values, return it immediately */
    if (n < 5)
        return init[n];

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL)  /* allocation failure – return 0 as safe fallback */
            return (seq_t)0;
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init[i];
    }

    /* Grow cache if needed */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)  /* allocation failure – keep old cache */
            return cache[n];  /* unchanged, but better than crash */
        cache = tmp;

        /* Compute missing terms using the linear recurrence */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            cache[i] =
                ( (seq_t)4 * cache[i-1]
                - (seq_t)2 * cache[i-2]
                - (seq_t)2 * cache[i-3]
                - (seq_t)5 * cache[i-4]
                + (seq_t)6 * cache[i-5] );
        }
        cache_size = new_size;
    }

    return cache[n];
}
