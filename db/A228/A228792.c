/*
 * A228792  Number of n X 4 binary arrays with top left element equal to 1 and
 *          no two ones adjacent horizontally or nw-se.
 * https://oeis.org/A228792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3, a(2)=16, a(3)=97, a(4)=586,
   a(n) = 8*a(n‑1) - 12*a(n‑2) + 4*a(n‑3) for n>4 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_up_to = 0;

    /* Return known values directly for the first four indices. */
    if (n <= 4) {
        switch ((int)n) {
            case 1: return (seq_t)3;
            case 2: return (seq_t)16;
            case 3: return (seq_t)97;
            case 4: return (seq_t)586;
        }
    }

    /* Initialise the cache on first call. */
    if (cache == NULL) {
        size_t init_size = 5;                /* indices 0..4 */
        cache = (seq_t *)calloc(init_size, sizeof(seq_t));
        if (cache == NULL) return 0;         /* allocation failure */
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)16;
        cache[3] = (seq_t)97;
        cache[4] = (seq_t)586;
        cached_up_to = 4;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > (size_t)cached_up_to + 1) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failure */
        cache = tmp;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        cache[i] = (cache[i-1] * (seq_t)8) - (cache[i-2] * (seq_t)12)
                   + (cache[i-3] * (seq_t)4);
    }
    cached_up_to = n;

    return cache[n];
}
