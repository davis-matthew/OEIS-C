/*
 * A158954  Numerator of Hermite(n, 1/4).
 * https://oeis.org/A158954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 20, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158954: Numerator of Hermite(n, 1/4)
   Recurrence: a(n) = a(n-1) - 8*(n-1)*a(n-2)
   Initial values: a(0) = 1, a(1) = 1
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t computed_max = 0;

    /* Handle the first two terms without allocating a cache */
    if (n == 0) return 1;
    if (n == 1) return 1;

    /* Allocate cache for the first two terms on first use */
    if (!cache) {
        cache_size = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) abort();                 /* out of memory */
        cache[0] = 1;
        cache[1] = 1;
        computed_max = 1;
    }

    /* If we already have the requested term, return it */
    if (n <= computed_max) return cache[n];

    /* Grow the cache if needed */
    while (cache_size <= (size_t)n) {
        size_t new_size = cache_size * 2;
        if (new_size <= cache_size) new_size = cache_size + (n - cache_size + 1);
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        idx_t mult = 8 * (i - 1);                   /* 8*(n-1) */
        seq_t next = cache[i - 1] - ((seq_t)mult) * cache[i - 2];
        cache[i] = next;
    }
    computed_max = n;
    return cache[n];
}
