/*
 * A172027  a(1) = 1; for n > 1, a(n) = smallest k such that a(n-1)^3 + k is a
 *          cube.
 * https://oeis.org/A172027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   a(n) = 1 + 3 * a(n-1) * (a(n-1) + 1)   for n > 1   (OEIS A172027) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* unspecified behaviour for n == 0 – return 0 */
    if (n == 0) return 0;

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements allocated (>= 2 if cache!=NULL) */

    /* Initialise cache on first call */
    if (!cache) {
        /* allocate indices 0 and 1; cache[0] unused */
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failed */
        cache[0] = 0;                      /* dummy value */
        cache[1] = 1;                      /* a(1) */
        cache_len = 2;
    }

    /* Grow cache if necessary and compute missing terms */
    size_t target = (size_t)n;
    if (target >= cache_len) {
        size_t new_len = target + 1;        /* indices 0 .. target */
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed */
        cache = tmp;
        for (size_t i = cache_len; i <= target; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = 1 + 3 * prev * (prev + 1);
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
