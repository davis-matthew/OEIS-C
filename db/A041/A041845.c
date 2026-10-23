/*
 * A041845  Denominators of continued fraction convergents to sqrt(444).
 * https://oeis.org/A041845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 590*a(n-2) - a(n-4) for n>3 a(0) = 1, a(1) = 14, a(2) =
 *            589, a(3) = 8260
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t max_n = (idx_t)-1;          /* last index computed */
    static size_t capacity = 0;              /* allocated size */

    /* initial allocation and fill base cases */
    if (cache == NULL) {
        capacity = (n >= 4 ? n + 1 : 4);
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache)
            return 0;                     /* allocation failure; caller should handle */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)589;
        cache[3] = (seq_t)8260;
        max_n = 3;
    }

    /* if requested term already computed, return it */
    if (n <= max_n)
        return cache[n];

    /* expand cache if necessary */
    if ((size_t)(n + 1) > capacity) {
        capacity = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (!tmp)
            return 0;                     /* allocation failure */
        cache = tmp;
    }

    /* compute new terms iteratively */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i <= 3)                     /* base cases are already set */
            continue;
        cache[i] = (seq_t)590 * cache[i - 2] - cache[i - 4];
    }
    max_n = n;
    return cache[n];
}
