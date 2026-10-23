/*
 * A163549  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*a(n-1)+27*a(n-2)+27*a(n-3)+27*a(n-4)-378*a(n-5)
 *            initial values: a(0)=1, a(1)=29, a(2)=812, a(3)=22736,
 *            a(4)=636608, a(5)=17824618
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* dynamic cache for computed values */
static size_t cache_size = 0;        /* number of values currently stored */
static size_t cache_cap   = 0;        /* allocated capacity of cache */

/* Initialise the cache with the first six values. */
static void init_cache(void)
{
    if (cache != NULL) return;           /* already initialised */

    /* Allocate initial capacity for the first six terms.  */
    cache_cap = 8;                       /* a bit more than needed */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (cache == NULL) exit(EXIT_FAILURE); /* abort on allocation failure */

    /* The first six terms of the sequence. All constants fit in seq_t. */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)29;
    cache[2] = (seq_t)812;
    cache[3] = (seq_t)22736;
    cache[4] = (seq_t)636608;
    cache[5] = (seq_t)17824618;

    cache_size = 6;         /* we have terms 0 .. 5 stored */
}

seq_t a(idx_t n)
{
    /* Ensure the cache is initialised. */
    init_cache();

    /* Return directly if we already have the desired term. */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Grow the cache to hold up to n. */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);   /* abort on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values using the recurrence. */
    for (size_t i = cache_size; i < needed; ++i) {
        /* Recurrence holds for i >= 6.  For i == 5 we already have the value. */
        if (i >= 6) {
            cache[i] =  (seq_t)27 * cache[i-1]
                       + (seq_t)27 * cache[i-2]
                       + (seq_t)27 * cache[i-3]
                       + (seq_t)27 * cache[i-4]
                       - (seq_t)378 * cache[i-5];
        } else {
            /* Should never reach here because cache already contains 0..5. */
            cache[i] = (seq_t)0;
        }
    }

    cache_size = needed;
    return cache[(size_t)n];
}
