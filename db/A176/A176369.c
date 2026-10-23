/*
 * A176369  y-values in the solution to x^2 - 65*y^2 = 1.
 * https://oeis.org/A176369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 258*a(n-1) - a(n-2), a(0) = 0, a(1) = 16. Offset: 1 (a(1)
 *            = a(0), a(2) = a(1))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* convert to recurrence index (a(n) corresponds to a_rec(n-1)) */
    idx_t rec = (idx_t)(n - 1);

    /* static cache for memoized values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initialise cache if empty */
    if (cache_size == 0) {
        /* allocate space for indices 0 and 1 */
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failed */
        cache[0] = (seq_t)0;                /* a(0) = 0 */
        cache[1] = (seq_t)16;               /* a(1) = 16 */
        cache_size = 2;
    }

    /* extend cache up to required index */
    if (rec + 1 > cache_size) {
        size_t new_size = rec + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        /* compute new values using recurrence */
        for (size_t i = cache_size; i <= rec; ++i) {
            cache[i] = (seq_t)258 * cache[i - 1] - cache[i - 2];
        }
        cache_size = new_size;
    }

    return cache[rec];
}
