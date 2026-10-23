/*
 * A041182  Numerators of continued fraction convergents to sqrt(102).
 * https://oeis.org/A041182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 202*a(n-2) - a(n-4); Initial values: a(0)=10, a(1)=101,
 *            a(2)=2030, a(3)=20401
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;          /* number of cached elements (indices 0..cached_max-1) */

    /* Allocate or grow cache to hold up to index n */
    if (cache == NULL) {
        cache = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */

        /* Initialise base cases */
        cache[0] = (seq_t)10;
        if (n >= 1) cache[1] = (seq_t)101;
        if (n >= 2) cache[2] = (seq_t)2030;
        if (n >= 3) cache[3] = (seq_t)20401;

        /* Compute remaining terms up to n */
        for (idx_t i = 4; i <= n; ++i)
            cache[i] = (seq_t)202 * cache[i - 2] - cache[i - 4];

        cached_max = n + 1;
    } else if (cached_max <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failed */
        cache = tmp;

        /* Ensure base cases are set */
        cache[0] = (seq_t)10;
        if (n >= 1) cache[1] = (seq_t)101;
        if (n >= 2) cache[2] = (seq_t)2030;
        if (n >= 3) cache[3] = (seq_t)20401;

        /* Compute missing terms from the current max to n */
        for (idx_t i = cached_max > 4 ? cached_max : 4; i <= n; ++i)
            cache[i] = (seq_t)202 * cache[i - 2] - cache[i - 4];

        cached_max = n + 1;
    }

    return cache[n];
}
