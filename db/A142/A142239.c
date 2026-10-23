/*
 * A142239  Denominators of continued fraction convergents to sqrt(3/2).
 * https://oeis.org/A142239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 10*a(n-2) - a(n-4) for n > 3 a(0) = 1, a(1) = 4, a(2) =
 *            9, a(3) = 40
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;   /* pointer to array of seq_t */
    static size_t cache_len = 0;  /* number of elements stored (max index + 1) */

    /* Initial allocation and base values */
    if (cache == NULL) {
        cache_len = 4;               /* indices 0..3 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) abort();

        cache[0] = 1;                /* a(0) */
        cache[1] = 4;                /* a(1) */
        cache[2] = 9;                /* a(2) */
        cache[3] = 40;               /* a(3) */
    }

    /* Convert idx_t to size_t for array indexing */
    size_t upto = (size_t)n;

    /* Grow cache if necessary and compute missing terms */
    if (upto >= cache_len) {
        size_t start = cache_len;    /* first new index to compute */
        seq_t *tmp = (seq_t *)realloc(cache, (upto + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;

        /* Compute terms using the recurrence */
        for (size_t i = start; i <= upto; ++i) {
            /* For i == 2 or 3 recurrence would refer to negative indices,
               but we only reach this loop with i >= 4. */
            cache[i] = cache[i - 2] * (seq_t)10 - cache[i - 4];
        }
        cache_len = upto + 1;
    }

    return cache[upto];
}
