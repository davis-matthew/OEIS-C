/*
 * A163461  a(n) = 18*a(n-1) - 79*a(n-2) for n > 1; a(0) = 1, a(1) = 10.
 * https://oeis.org/A163461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) - 79*a(n-2), a(0) = 1, a(1) = 10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage function requested by the exercise */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* index of the highest computed element */

    /* Initialise cache with the first two terms if it hasn't been done yet */
    if (cache == NULL) {
        /* Allocate space for at least two elements; we may grow it later */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;     /* allocation failure */
        cache[0] = (seq_t)1;                    /* a(0) */
        cache[1] = (seq_t)10;                   /* a(1) */
        cache_len = 1;                          /* highest computed index so far */
    }

    /* Grow cache and compute missing terms up to n */
    if (n > cache_len) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;       /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            cache[i] = cache[i-1] * (seq_t)18 - cache[i-2] * (seq_t)79;
        }
        cache_len = n;
    }

    return cache[n];
}
