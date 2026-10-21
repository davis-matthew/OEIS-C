/*
 * A203901  a(n)=f(a(n-1)+1,a(n-2)+1,a(n-3)), where f(x,y,z)=yz+zx+xy and
 *          (a(1),a(2),a(3))=(0,0,1).
 * https://oeis.org/A203901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(a(n-1)+1, a(n-2)+1, a(n-3)) where f(x,y,z) = yz + zx +
 *            xy Initial values: a(1)=0, a(2)=0, a(3)=1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#if defined(__cplusplus)
    /* use C++ memory functions for arbitrary‑precision types */
    #include <new>
#endif

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 as a safe default */
    if (n == 0) return (seq_t)0;

    /* static cache state */
    static seq_t *cache = NULL;      /* array of a(1)..a(cache_last)    */
    static idx_t cache_last = 0;     /* largest n computed so far      */
    static size_t cache_cap   = 0;   /* allocated capacity of cache     */

    /* Initialize the cache with the first three terms */
    if (cache_cap == 0) {
        size_t need = 3;
    #if defined(__cplusplus)
        cache = new seq_t[need];
    #else
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!cache) abort();
    #endif
        cache[0] = (seq_t)0;   /* a(1) */
        cache[1] = (seq_t)0;   /* a(2) */
        cache[2] = (seq_t)1;   /* a(3) */
        cache_last = 3;
        cache_cap   = need;
    }

    /* If the value is already cached, return it */
    if (n <= cache_last) {
        return cache[n - (idx_t)1];
    }

    /* Grow the cache if necessary */
    size_t new_cap = (size_t)n;
    if (new_cap > cache_cap) {
    #if defined(__cplusplus)
        seq_t *tmp = new seq_t[new_cap];
        for (idx_t i = 0; i < cache_last; ++i) {
            tmp[i] = cache[i];
        }
        delete[] cache;
        cache = tmp;
    #else
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    #endif
        cache_cap = new_cap;
    }

    /* Compute terms from cache_last+1 up to n */
    for (idx_t i = cache_last + (idx_t)1; i <= n; ++i) {
        /* indices: i-1 -> cache[i-2], i-2 -> cache[i-3], i-3 -> cache[i-4] */
        seq_t x = cache[i - (idx_t)2] + (seq_t)1; /* a(i-1)+1 */
        seq_t y = cache[i - (idx_t)3] + (seq_t)1; /* a(i-2)+1 */
        seq_t z = cache[i - (idx_t)4];           /* a(i-3)   */
        cache[i - (idx_t)1] = y * z + z * x + x * y;
    }

    cache_last = n;
    return cache[n - (idx_t)1];
}
