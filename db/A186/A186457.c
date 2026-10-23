/*
 * A186457  Number of (n+1) X 5 binary arrays with every 2 X 2 subblock
 *          commuting with each of its horizontal and vertical 2 X 2 subblock
 *          neighbors.
 * https://oeis.org/A186457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - a(n-5) (n > 10) Initial
 *            values (offset 1): a(1)=61, a(2)=88, a(3)=162, a(4)=309,
 *            a(5)=449, a(6)=673, a(7)=945, a(8)=1493, a(9)=2201, a(10)=3473
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 43, u64 n <= 97, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ == 16
    typedef unsigned __int128 u128_t;
#else
    typedef unsigned long long u128_t;
#endif

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cached = 0;   /* highest index computed */
    static size_t cap    = 0;   /* allocated capacity (elements) */

    /* Initialise cache with first 10 terms (indices 0..10) */
    if (cache == NULL) {
        cap = 11;                              /* 0 .. 10 inclusive */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;                  /* allocation failed */

        cache[0] = 0;                          /* unused placeholder */
        cache[1] = 61; cache[2] = 88;
        cache[3] = 162; cache[4] = 309;
        cache[5] = 449; cache[6] = 673;
        cache[7] = 945; cache[8] = 1493;
        cache[9] = 2201; cache[10] = 3473;

        cached = 10;
    }

    /* Grow cache if necessary */
    if (n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                     /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n > cached) {
        for (idx_t i = cached + 1; i <= n; ++i) {
            if (i <= 10) {
                continue;                       /* already initialised */
            }
            /* Use a wider type to avoid overflow during the computation */
            u128_t a1 = (u128_t)cache[i - 1];
            u128_t a3 = (u128_t)cache[i - 3];
            u128_t a4 = (u128_t)cache[i - 4];
            u128_t a5 = (u128_t)cache[i - 5];

            u128_t val = 2 * a1 - 2 * a3 + 2 * a4 - a5;
            cache[i] = (seq_t)val;              /* truncation is fine for types that cannot hold the value */
        }
        cached = n;
    }

    return cache[n];                              /* return the requested term */
}
