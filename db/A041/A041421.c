/*
 * A041421  Denominators of continued fraction convergents to sqrt(226).
 * https://oeis.org/A041421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 30*a(n-1) + a(n-2); a(0)=1, a(1)=30
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t size = 0; /* number of elements currently stored */

    /* initialise cache with first two terms */
    if (size == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 2);
        cache[0] = 1;
        cache[1] = 30;
        size   = 2;
    }

    /* return cached value if available */
    if (n < size)
        return cache[n];

    /* enlarge cache to hold terms up to n */
    cache = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));

    /* compute missing terms using the recurrence */
    for (idx_t i = size; i <= n; ++i)
        cache[i] = cache[i-1] * 30 + cache[i-2];

    size = n + 1;
    return cache[n];
}
