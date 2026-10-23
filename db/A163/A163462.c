/*
 * A163462  a(n) = 20*a(n-1) - 98*a(n-2) for n > 1; a(0) = 1, a(1) = 11.
 * https://oeis.org/A163462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163462: a(n) = 20*a(n-1) - 98*a(n-2), a(0)=1, a(1)=11 */
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;      /* array of previously computed terms */
    static size_t  size   = 0;         /* number of elements stored (cache[0..size-1]) */
    static size_t  cap    = 0;         /* allocated capacity */

    /* handle the first two terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    /* initialise the cache on the first call */
    if (cache == NULL) {
        cap = 2;
        size = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        /* initialise the first two elements */
#ifdef __cplusplus
        new (&cache[0]) seq_t(1);
        new (&cache[1]) seq_t(11);
#else
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
#endif
    }

    /* grow and compute as necessary */
    if ((size_t)n >= size) {
        /* grow capacity */
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cap   = new_cap;

        /* construct any newly allocated elements (C++ only) */
#ifdef __cplusplus
        for (size_t i = size; i < cap; ++i)
            new (&cache[i]) seq_t();
#endif

        /* compute missing terms */
        for (size_t i = size; i <= (size_t)n; ++i) {
            cache[i] = ( (seq_t)20 * cache[i - 1] - (seq_t)98 * cache[i - 2] );
        }
        size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
