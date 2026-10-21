/*
 * A201008  Triangular numbers, T(m), that are five-sixths of another
 *          triangular number: T(m) such that 6*T(m) = 5*T(k) for some k.
 * https://oeis.org/A201008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 55, a(n) = 482*a(n-1) - a(n-2) + 55 for n>1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;   /* pointer to array of seq_t */
    static size_t cap   = 0;      /* total capacity of the array          */
    static size_t len   = 0;      /* number of elements currently valid (<=cap) */

    /* Initial allocation */
    if (cap == 0) {
        const size_t init_cap = 2;
#ifdef __cplusplus
        cache = new seq_t[init_cap];
#else
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0; /* fallback if allocation fails */
#endif
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)55;
        cap = init_cap;
        len = 2;
    }

    /* Expand capacity if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap * 2;
        while ((size_t)n + 1 > newcap) newcap *= 2;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[newcap];
#else
        seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!tmp) return 0; /* fallback if allocation fails */
#endif
        /* Copy existing values */
        for (size_t i = 0; i < len; ++i) tmp[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = tmp;
        cap   = newcap;
    }

    /* Compute missing terms up to index n */
    if ((size_t)n >= len) {
        for (size_t i = len; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)0;
            } else if (i == 1) {
                cache[1] = (seq_t)55;
            } else {
                /* Recurrence: a(i) = 482*a(i-1) - a(i-2) + 55 */
                cache[i] = 482 * cache[i-1] - cache[i-2] + 55;
            }
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
