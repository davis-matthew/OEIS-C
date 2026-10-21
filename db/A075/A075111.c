/*
 * A075111  a(n) = Sum_{i=0..floor(n/2)} (-1)^(i+floor(n/2))*T(2i+e), where
 *          T(n) are tribonacci numbers (A000073) and e = (1/2)(1-(-1)^n).
 * https://oeis.org/A075111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) + a(n-4) + a(n-5), a(0) = 0, a(1) = 1,
 *            a(2) = 1, a(3) = 1, a(4) = 3; offset 0
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes A075111 for any non‑negative n.
   It memoises results in a static array so repeated calls are O(1).
   The routine uses only the basic arithmetic operators required
   for type seq_t.  No external libraries are needed. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* current number of terms stored */
    static size_t cache_cap = 0;       /* current allocation capacity */

    /* Ensure the cache is initialised with the first five terms */
    if (cache == NULL) {
        cache_cap = 8;                 /* start with some spare room */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)             /* allocation failed – abort */
            return 0;

        /* first five terms of the sequence */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 3;
        cache_size = 5;
    }

    /* If the requested index is already computed, return it immediately */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Compute missing terms up to n */
    while (cache_size <= (size_t)n) {
        /* Grow the cache if necessary */
        if (cache_size >= cache_cap) {
            /* Double the capacity until it can hold the new element */
            do { cache_cap <<= 1; } while (cache_size >= cache_cap);
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (cache == NULL)         /* allocation failed – abort */
                return 0;
        }

        /* Recurrence: a[i] = a[i-1] + 2*a[i-3] + a[i-4] + a[i-5] */
        size_t i = cache_size;
        seq_t val = cache[i-1] + 2*cache[i-3] + cache[i-4] + cache[i-5];
        cache[i] = val;
        cache_size++;
    }

    return cache[(size_t)n];
}
