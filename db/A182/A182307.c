/*
 * A182307  a(n+1) = a(n) + floor(a(n)/6) with a(0) = 6.
 * https://oeis.org/A182307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + floor(a(n-1)/6), a(0) = 6
 * Range:     u8 n <= 26, u16 n <= 62, u32 n <= 134, u64 n <= 278,
 *            u128 n <= 566, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static memoization cache */
    static seq_t *cache = NULL;   /* array of computed terms   */
    static size_t cache_cap = 0;  /* allocated capacity       */
    static size_t cache_len = 0;  /* number of terms stored   */

    /* Initialise the cache on first call */
    if (cache == NULL) {
        cache_cap = 16;                          /* initial slots          */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();              /* allocation failed      */
        cache[0] = (seq_t)6;                     /* a(0) = 6               */
        cache_len = 1;
    }

    /* Grow the cache until it contains a(n) */
    while (cache_len <= (size_t)n) {
        /* Ensure there is space for the next element */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap << 1;     /* double capacity       */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();           /* reallocation failed   */
            cache = tmp;
            cache_cap = new_cap;
        }

        seq_t prev = cache[cache_len - 1];
        seq_t add  = prev / 6;                   /* floor division (unsigned) */
        cache[cache_len] = prev + add;           /* recurrence step          */

        cache_len++;
    }

    return cache[(size_t)n];
}
