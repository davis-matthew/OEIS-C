/*
 * A235843  Number of (n+1) X (1+1) 0..3 arrays with the upper median unequal
 *          to the lower median in every 2 X 2 subblock.
 * https://oeis.org/A235843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A235843  :  Number of (n+1)×(1+1) 0..3 arrays with the upper median unequal to the lower median
    in every 2×2 subblock.  The sequence satisfies the 5‑term linear recurrence

        a(n) = 14*a(n-1) - 31*a(n-2) - 94*a(n-3) + 192*a(n-4) + 192*a(n-5)

    with initial terms

        a(1)=156,  a(2)=1584,  a(3)=16228,  a(4)=166816,  a(5)=1716484.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  The 5 known starting values.  */
    static const seq_t init[5] = {156, 1584, 16228, 166816, 1716484};

    /*  Static cache of computed terms.  cache[0] corresponds to a(1).  */
    static seq_t *cache      = NULL;   /* pointer to the array */
    static idx_t cache_size  = 0;      /* number of elements that are allocated */
    static idx_t computed    = 5;      /* highest index that has been computed (1‑based) */

    /*  Fast answer for n ≤ 5.  */
    if (n <= 5)
        return init[n - 1];

    /*  Allocate the array if this is the first call, or enlarge it if
        the requested index is beyond the current capacity.  */
    if (cache == NULL) {
        /* First allocation: at least as large as the requested n,
           but at least 5 because we need to store the initial values. */
        cache_size = (idx_t)(n > 5 ? n : 5);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failed – not expected in the tests */
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = init[i];
    }
    else if (n > cache_size) {
        /* Grow the array to the requested size.  Use realloc with a cast
           for C++ compatibility.  */
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed – not expected */
        cache = tmp;
        cache_size = n;
    }

    /*  If the value has already been computed, return it immediately.  */
    if (n <= computed)
        return cache[n - 1];

    /*  Compute missing terms from the next index up to n.  */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = (seq_t)14 * cache[i - 2]            /* a(i-1)   */
                  - (seq_t)31 * cache[i - 3]            /* a(i-2)   */
                  - (seq_t)94 * cache[i - 4]            /* a(i-3)   */
                  + (seq_t)192 * cache[i - 5]           /* a(i-4)   */
                  + (seq_t)192 * cache[i - 6];          /* a(i-5)   */
        cache[i - 1] = val;
    }
    computed = n;
    return cache[n - 1];
}
