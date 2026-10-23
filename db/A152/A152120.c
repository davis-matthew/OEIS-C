/*
 * A152120  a(n) = 2^n * Product_{k=1..(n-1)/2} (2 + 3*cos(k*Pi/n)^2).
 * https://oeis.org/A152120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-2) - 9*a(n-4) for n > 4, with initial values
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=22, a(4)=56
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* For the very first five terms return them directly. */
    if (n < 5) {
        switch ((int)n) {
            case 0: return 1;
            case 1: return 2;
            case 2: return 4;
            case 3: return 22;
            case 4: return 56;
            default: /* unreachable */ return 0;
        }
    }

    /* Static cache keeps all values computed so far. */
    static seq_t *cache = NULL;
    static size_t cached_upto = 0;   /* largest index that is cached */

    /* First allocation: we need space for indices 0 … n. */
    if (cache == NULL) {
        size_t new_size = n + 1;
        seq_t *new_cache =
#ifdef __cplusplus
            new seq_t[new_size];
#else
            (seq_t *)malloc(new_size * sizeof(seq_t));
#endif
        if (!new_cache) abort();

        /* Initialise the first five values. */
        new_cache[0] = 1;
        new_cache[1] = 2;
        new_cache[2] = 4;
        new_cache[3] = 22;
        new_cache[4] = 56;

        /* Compute any remaining values needed up to n. */
        for (idx_t i = 5; i <= n; ++i) {
            new_cache[i] =
                (seq_t)14 * new_cache[i - 2] - (seq_t)9 * new_cache[i - 4];
        }

        cache = new_cache;
        cached_upto = n;
        return cache[n];
    }

    /* Extend the cache when a larger index is requested. */
    if (cached_upto < (size_t)n) {
        size_t new_size = n + 1;
        seq_t *ext_cache =
#ifdef __cplusplus
            new seq_t[new_size];
#else
            (seq_t *)realloc(cache, new_size * sizeof(seq_t));
#endif
        if (!ext_cache) abort();

        /* If we used realloc in C, the existing data were copied
           automatically. For C++ new allocates a fresh array, so we
           need to copy the existing part ourselves. */
#ifndef __cplusplus
        /* nothing to do – realloc already copied the data */
#else
        memcpy(ext_cache, cache, (cached_upto + 1) * sizeof(seq_t));
#endif

        /* Compute new terms from the first missing index. */
        for (idx_t i = cached_upto + 1; i <= n; ++i) {
            ext_cache[i] =
                (seq_t)14 * ext_cache[i - 2] - (seq_t)9 * ext_cache[i - 4];
        }

        cache = ext_cache;
        cached_upto = n;
    }

    return cache[n];
}
