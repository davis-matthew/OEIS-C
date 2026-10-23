/*
 * A143419  G.f.: 1/p(x), where p(x) = degree 22 Salem polynomial p(x) = x^22
 *          + x^21 - x^19 - 2*x^18 - 3*x^17 - 3*x^16 - 2*x^15 + 2*x^13 +
 *          4*x^12 + 5*x^11 + 4*x^10 + 2*x^9 - 2*x^7 - 3*x^6 - 3*x^5 - 2*x^4 -
 *          x^3 + x + 1.
 * https://oeis.org/A143419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1) + a(n-3) + 2*a(n-4) + 3*a(n-5) + 3*a(n-6) +
 *            2*a(n-7) - 2*a(n-9) - 4*a(n-10) - 5*a(n-11) - 4*a(n-12) -
 *            2*a(n-13) + 2*a(n-15) + 3*a(n-16) + 3*a(n-17) + 2*a(n-18) +
 *            a(n-19) - a(n-21) - a(n-22) with a(0)=1, a(1)=-1, a(2)=1,
 *            a(3)=0, a(4)=1, a(5)=1, a(6)=1, a(7)=2, a(8)=2, a(9)=4,
 *            a(10)=...
 * Range:     i8 n <= 20, i16 n <= 37, i32 n <= 72, i64 n <= 142,
 *            i128 n <= 282, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>   /* for operator new and delete */
#endif

seq_t a(idx_t n)
{
    /* static cache to store already computed values      */
    static seq_t *cache      = NULL;     /* array of values              */
    static idx_t last_index  = 0;        /* index of last cached value   */
    static size_t capacity   = 0;        /* allocated size of cache      */

    /* helper to initialise the cache with the first 24 values */
    if (cache == NULL) {
        /* initial values from the OEIS entry */
        const seq_t init[24] = {
            1,  -1,   1,   0,   1,   1,   1,   2,   2,   4,
            4,   7,   9,  12,  17,  23,  32,  44,  60,  83,
            113, 156, 214, 294
        };
        capacity = 24;
#ifdef __cplusplus
        cache = new seq_t[capacity];
#else
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
#endif
        if (!cache) abort();            /* out of memory */
        for (size_t i = 0; i < 24; ++i) cache[i] = init[i];
        last_index = 23;                /* we have values up to 23 */
    }

    /* enlarge cache if needed */
    if (n > (idx_t)(capacity - 1)) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < capacity * 2) new_cap = capacity * 2;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < capacity; ++i) tmp[i] = cache[i];
        delete[] cache;
#else
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
#endif
        cache = tmp;
        capacity = new_cap;
    }

    /* compute missing values up to n */
    if (n > last_index) {
        for (idx_t k = last_index + 1; k <= n; ++k) {
            size_t ik = (size_t)k;
            seq_t val = -cache[ik - 1]
                        + cache[ik - 3]
                        + 2 * cache[ik - 4]
                        + 3 * cache[ik - 5]
                        + 3 * cache[ik - 6]
                        + 2 * cache[ik - 7]
                        - 2 * cache[ik - 9]
                        - 4 * cache[ik - 10]
                        - 5 * cache[ik - 11]
                        - 4 * cache[ik - 12]
                        - 2 * cache[ik - 13]
                        + 2 * cache[ik - 15]
                        + 3 * cache[ik - 16]
                        + 3 * cache[ik - 17]
                        + 2 * cache[ik - 18]
                        +   cache[ik - 19]
                        -   cache[ik - 21]
                        -   cache[ik - 22];
            cache[ik] = val;
        }
        last_index = n;
    }

    return cache[(size_t)n];
}
