/*
 * A205066  Number of (n+1) X 3 0..1 arrays with the number of rightwards and
 *          downwards edge increases in each 2 X 2 subblock differing from the
 *          number in all its horizontal and vertical neighbors.
 * https://oeis.org/A205066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-2) - a(n-6) - 4*a(n-8) - a(n-10) + a(n-12)  (n > 14)
   Initial values: a(1)=40, a(2)=64, a(3)=90, a(4)=146, a(5)=244,
   a(6)=386, a(7)=638, a(8)=1018, a(9)=1666, a(10)=2676,
   a(11)=4360, a(12)=7024, a(13)=11418, a(14)=18420 */
#include <stdlib.h>

/* The function implements the recurrence above and memoises results
   up to the largest n seen. It is safe for any call order, n ≥ 1. */
seq_t a(idx_t n)
{
    /* Base values for n = 1 … 14 */
    static const seq_t base[14] = {
        40,   /* 1  */
        64,   /* 2  */
        90,   /* 3  */
        146,  /* 4  */
        244,  /* 5  */
        386,  /* 6  */
        638,  /* 7  */
        1018, /* 8  */
        1666, /* 9  */
        2676, /*10 */
        4360, /*11 */
        7024, /*12 */
        11418,/*13 */
        18420 /*14 */
    };

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of filled terms (a(1) … a(cache_len)) */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* Initialise cache on first invocation */
    if (cache == NULL) {
        /* Allocate at least space for the first 14 terms */
        cache_cap = 14;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 14; ++i) cache[i] = base[i];
        cache_len = 14;
    }

    /* Extend cache if needed */
    if ((size_t)n > cache_len) {
        if ((size_t)n > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
            if (tmp == NULL) { /* allocation failed; return 0 as a safe fallback */
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = (size_t)n;
        }
        /* Compute missing terms up to n */
        for (size_t k = cache_len + 1; k <= (size_t)n; ++k) {
            seq_t value;
            if (k <= 14) {
                value = base[k - 1];
            } else {
                /* Recurrence: a(k) = 3*a(k-2) - a(k-6) - 4*a(k-8) - a(k-10) + a(k-12) */
                value = 3 * cache[k - 3]          /* a(k-2) */
                      -          cache[k - 7]      /* a(k-6) */
                      - 4 * cache[k - 9]          /* a(k-8) */
                      -          cache[k - 11]     /* a(k-10) */
                      +          cache[k - 13];    /* a(k-12) */
            }
            cache[k - 1] = value;
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
