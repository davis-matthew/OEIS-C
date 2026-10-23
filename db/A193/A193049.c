/*
 * A193049  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A193049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            7*a(n-1)-20*a(n-2)+29*a(n-3)-20*a(n-4)+a(n-5)+8*a(n-6)-5*a(n-7)+a(n-8),
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=4, a(5)=12, a(6)=37,
 *            a(7)=105
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following function is written in standard C11/C++17, compiling
   for both builtin integer types and arbitrary‑precision integer
   classes.  It uses memoisation with a static cache that grows
   dynamically as needed.  No other external dependencies are
   required. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0 .. 7 */
    static const seq_t init_vals[8] = {0, 1, 1, 2, 4, 12, 37, 105};

    /* Static cache structures */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* current capacity of cache */
    static idx_t  cache_last = 7;      /* highest index already computed */

    /* Ensure the cache exists and is big enough.  The first
       allocation also copies the initial values into cache. */
    if (cache == NULL) {
        const size_t INIT = 8;
        cache = (seq_t*)malloc(INIT * sizeof(seq_t));
        for (size_t i = 0; i < INIT; ++i)
            cache[i] = init_vals[i];
        cache_size = INIT;
    }
    if ((size_t)n >= cache_size) {
        size_t new_size = n + 1;
        if (new_size < cache_size * 2)
            new_size = cache_size * 2;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* Return known initial values directly. */
    if (n < 8)
        return init_vals[n];

    /* Compute missing terms up to n. */
    for (idx_t k = cache_last + 1; k <= n; ++k) {
#if defined(__cplusplus)
        /* In C++ the overloaded operators of seq_t handle big integers
           automatically.  We cast the small integer constants to seq_t
           to force the correct type. */
        cache[k] =
            (seq_t)7  * cache[k-1] -
            (seq_t)20 * cache[k-2] +
            (seq_t)29 * cache[k-3] -
            (seq_t)20 * cache[k-4] +
            (seq_t)1  * cache[k-5] +
            (seq_t)8  * cache[k-6] -
            (seq_t)5  * cache[k-7] +
            (seq_t)1  * cache[k-8];
#else
        /* In C we compute in the wider __int128 type to avoid overflow. */
        __int128 sum = 0;
        sum += (__int128)7  * cache[k-1];
        sum -= (__int128)20 * cache[k-2];
        sum += (__int128)29 * cache[k-3];
        sum -= (__int128)20 * cache[k-4];
        sum += (__int128)1  * cache[k-5];
        sum += (__int128)8  * cache[k-6];
        sum -= (__int128)5  * cache[k-7];
        sum += (__int128)1  * cache[k-8];
        cache[k] = (seq_t)sum;
#endif
    }

    cache_last = n;
    return cache[n];
}
