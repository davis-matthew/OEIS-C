/*
 * A012781  Take every 5th term of Padovan sequence A000931, beginning with
 *          the second term.
 * https://oeis.org/A012781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 5*a(n+2) - 4*a(n+1) + a(n) a(0) = 0, a(1) = 1, a(2) = 4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values for all n seen so far */
static size_t cache_size = 0;    /* current length of cache */

seq_t a(idx_t n) {
    size_t idx = (size_t)n;                 /* use size_t for array indexing */

    /* Handle the first three values directly if they have not been cached yet */
    if (idx < 3) {
        static const seq_t init[3] = {0, 1, 4};
        return init[idx];
    }

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failure – unlikely */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 4;
    }

    /* Grow cache if necessary and compute any missing terms */
    if (idx >= cache_size) {
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – unlikely */
        cache = tmp;
        for (size_t i = cache_size; i <= idx; ++i) {
            cache[i] = 5 * cache[i-1] - 4 * cache[i-2] + cache[i-3];
        }
        cache_size = new_size;
    }

    return cache[idx];
}
