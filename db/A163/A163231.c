/*
 * A163231  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 43*a(n-1)+43*a(n-2)+43*a(n-3)-946*a(n-4) with a(0)=1,
 *            a(1)=45, a(2)=1980, a(3)=87120, a(4)=3832290
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t may be an arbitrary‑precision type; operations
   are performed using the public operators provided by that type. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[5] = {
        (seq_t)1,                     /* a(0) */
        (seq_t)45,                    /* a(1) */
        (seq_t)1980,                  /* a(2) */
        (seq_t)87120,                 /* a(3) */
        (seq_t)3832290                /* a(4) */
    };

    /* Cached values for indices previously computed. */
    static seq_t *cache = NULL;          /* dynamic array */
    static idx_t last_computed = (idx_t)-1; /* highest index stored */
    static size_t cache_size = 0;        /* allocated size of cache */

    /* Return early for the first five terms. */
    if (n < 5)
        return init[n];

    /* Initialise cache on the first call. */
    if (!cache) {
        cache_size = 5; /* at least five slots */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure – unlikely */
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = init[i];
        last_computed = 4;
    }

    /* Grow cache if necessary. */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = cache_size ? cache_size : 5;
        while (new_size <= (size_t)n)
            new_size <<= 1; /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i < 5) {
            cache[i] = init[i];        /* safety check */
        } else {
            /* Use explicit casts to avoid ambiguous operator overloads. */
            cache[i] = (seq_t)43 * cache[i-1]
                     + (seq_t)43 * cache[i-2]
                     + (seq_t)43 * cache[i-3]
                     - (seq_t)946 * cache[i-4];
        }
    }
    last_computed = n;

    return cache[n];
}
