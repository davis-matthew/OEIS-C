/*
 * A219515  Number of n X 4 arrays of the minimum value of corresponding
 *          elements and their horizontal, diagonal or antidiagonal neighbors
 *          in a random, but sorted with lexicographically nondecreasing rows
 *          and columns, 0..1 n X 4 array.
 * https://oeis.org/A219515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) for n>15
 *            Initial values: a(1)=4, a(2)=5, a(3)=24, a(4)=68, a(5)=187,
 *            a(6)=465, a(7)=1090, a(8)=2430, a(9)=5181, a(10)=10575,
 *            a(11)=20714, a(12)=39046, a(13)=71023, a(14)=124990,
 *            a(15)=213360
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 50, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free, size_t */
#include <stdbool.h>  /* bool */

seq_t a(idx_t n)
{
    /* initial values a(1)..a(15) */
    static const seq_t init_vals[15] = {
        (seq_t)4,   (seq_t)5,   (seq_t)24,  (seq_t)68,
        (seq_t)187, (seq_t)465, (seq_t)1090, (seq_t)2430,
        (seq_t)5181,(seq_t)10575,(seq_t)20714,(seq_t)39046,
        (seq_t)71023,(seq_t)124990,(seq_t)213360
    };

    /* static cache of computed terms a(1)..a(cache_len) */
    static seq_t *cache    = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static bool initialized = false;

    /* handle n <= 15 directly */
    if (n <= 15) {
        return init_vals[(size_t)n - 1];
    }

    /* one‑time initialization of cache with the 15 known values */
    if (!initialized) {
        /* start with sufficient capacity */
        cache_cap = ((size_t)n > (size_t)32) ? (size_t)n : 32;
        /* allocate */
#if defined(__cplusplus)
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
#endif
        if (!cache) {
            return (seq_t)0;   /* allocation failure; caller must handle */
        }
        for (size_t i = 0; i < 15; ++i) {
            cache[i] = init_vals[i];
        }
        cache_len = 15;
        initialized = true;
    }

    /* grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
#if defined(__cplusplus)
        seq_t *new_cache = new seq_t[new_cap];
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        delete [] cache;
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            return (seq_t)0;   /* allocation failure */
        }
        cache = new_cache;
#endif
        cache_cap = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    for (size_t idx = cache_len; idx < (size_t)n; ++idx) {
        /* idx is zero based for a(idx+1), recurrence valid because idx+1 > 15 */
        seq_t val =
              (seq_t)9   * cache[idx-1]
            - (seq_t)36  * cache[idx-2]
            + (seq_t)84  * cache[idx-3]
            - (seq_t)126 * cache[idx-4]
            + (seq_t)126 * cache[idx-5]
            - (seq_t)84  * cache[idx-6]
            + (seq_t)36  * cache[idx-7]
            - (seq_t)9   * cache[idx-8]
            + (seq_t)1   * cache[idx-9];
        cache[idx] = val;
    }
    cache_len = (size_t)n;
    return cache[n-1];
}
