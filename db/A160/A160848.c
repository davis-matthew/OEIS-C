/*
 * A160848  Number of lines through at least 2 points of an 8 X n grid of
 *          points.
 * https://oeis.org/A160848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 67, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A160848 – Number of lines through at least 2 points of an 8×n grid.
   Recurrence for n > 21:
       a(n) = -a(n-2) + a(n-5) + a(n-6) + 2*a(n-7) + a(n-8) + a(n-9)
              - a(n-11) - a(n-12) - 2*a(n-13) - a(n-14) - a(n-15)
              + a(n-18) + a(n-20)
   Initial values a(0)…a(21) are listed in the OEIS entry. */

#include <stdlib.h>
#include <string.h>

/* seq_t and idx_t are provided by the build system.
   They are unsigned integer types (possibly arbitrary‑precision).   */

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Ensure the cache can hold index n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_size) {
        seq_t *ptr = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!ptr) return (seq_t)0;   /* allocation failure → 0 */
        /* zero new elements */
        if (cache_size < needed)
            memset(ptr + cache_size, 0, (needed - cache_size) * sizeof(seq_t));
        cache = ptr;
        cache_size = needed;
    }

    /* Initialise the first 22 elements with the known values */
    static const seq_t init[22] = {
        (seq_t)0, (seq_t)1, (seq_t)66, (seq_t)131, (seq_t)238,
        (seq_t)361, (seq_t)534, (seq_t)709, (seq_t)938, (seq_t)1183,
        (seq_t)1470,(seq_t)1759,(seq_t)2104,(seq_t)2459,(seq_t)2870,
        (seq_t)3287,(seq_t)3740,(seq_t)4209,(seq_t)4734,(seq_t)5261,
        (seq_t)5844,(seq_t)6437
    };

    for (size_t i = 0; i < 22 && i < cache_size; ++i)
        cache[i] = init[i];

    /* Compute missing values using the recurrence */
    for (size_t i = 22; i < cache_size; ++i) {
        /* positive contributions */
        seq_t pos = cache[i-5];
        pos += cache[i-6];
        pos += cache[i-7] * (seq_t)2;
        pos += cache[i-8];
        pos += cache[i-9];
        pos += cache[i-18];
        pos += cache[i-20];

        /* negative contributions */
        seq_t neg = cache[i-2];
        neg += cache[i-11];
        neg += cache[i-12];
        neg += cache[i-13] * (seq_t)2;
        neg += cache[i-14];
        neg += cache[i-15];

        /* a(i) = pos - neg */
        cache[i] = pos - neg;
    }

    return cache[(size_t)n];
}
