/*
 * A168071  Expansion of (1-3*x^2-4*x^3)/((1-x)^2*(1+x+x^2)).
 * https://oeis.org/A168071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) (n > 3) Initial values: a(0) =
 *            1, a(1) = 1, a(2) = -2, a(3) = -5
 * Range:     i8 n <= 65, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* cached values of the sequence      */
static idx_t cache_size = 0;       /* number of elements stored          */

seq_t a(idx_t n)
{
    /* Initialise cache with the first four values if needed */
    if (cache_size == 0) {
        idx_t init_size = (n > 3) ? n + 1 : 4;
        cache = (seq_t*)malloc(sizeof(seq_t) * (size_t)init_size);
        if (!cache) exit(1);          /* allocation failed */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = (seq_t)-2;
        cache[3] = (seq_t)-5;
        cache_size = 4;
    }

    /* Extend cache to index n if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (size_t)new_size);
        if (!tmp) exit(1);            /* allocation failed */
        cache = tmp;
        /* Compute new terms using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i-1] + cache[i-3] - cache[i-4];
        }
        cache_size = new_size;
    }

    return cache[n];
}
