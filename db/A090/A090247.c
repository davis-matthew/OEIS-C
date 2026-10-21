/*
 * A090247  a(n) = 26*a(n-1) - a(n-2), starting with a(0) = 2 and a(1) = 26.
 * https://oeis.org/A090247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 26*a(n-1) - a(n-2)   with   a(0) = 2, a(1) = 26
 */
#include <stddef.h>  /* size_t */
#include <stdlib.h>  /* malloc, free */

seq_t a(idx_t n)
{
    /* Handle the trivial initial cases. */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)26;

    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;        /* array size allocated */
    static idx_t last = 1;             /* highest n for which cache is filled */

    /* Allocate / grow the cache on the first call. */
    if (cache == NULL) {
        capacity = 2;
        /* Use operator new[] in C++ to invoke constructors, otherwise malloc. */
#if defined(__cplusplus)
        cache = new seq_t[capacity];
#else
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
#endif
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)26;
        last = 1;
    }

    /* Grow cache if needed to reach n. */
    if ((size_t)n > (size_t)last) {
        size_t needed = (size_t)n + 1;
        if (needed > capacity) {
            size_t new_cap = needed;
#if defined(__cplusplus)
            seq_t *tmp = new seq_t[new_cap];
            for (size_t i = 0; i <= (size_t)last; ++i) tmp[i] = cache[i];
            delete[] cache;
            cache = tmp;
#else
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
#endif
            capacity = new_cap;
        }

        /* Iteratively fill missing terms. */
        for (size_t i = (size_t)last + 1; i <= (size_t)n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)26 - cache[i - 2];
        }
        last = (idx_t)n;
    }

    return cache[(size_t)n];
}
