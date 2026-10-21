/*
 * A052530  a(n) = 4*a(n-1) - a(n-2), with a(0) = 0, a(1) = 2.
 * https://oeis.org/A052530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4*a(n-1) - a(n-2), a(0) = 0, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for recurrence values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* allocated size */
    static idx_t computed_up_to = 0;  /* highest n already computed */

    /* first call: initialise cache for n = 0 and n = 1 */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(1);           /* allocation failure */
        cache[0] = 0;
        cache[1] = 2;
        computed_up_to = 1;
    }

    /* if the requested term is already in cache, return it */
    if (n <= computed_up_to)
        return cache[n];

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = cache_size * 2;
        if (new_size <= n) new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);            /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing values using the recurrence */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        cache[i] = 4 * cache[i-1] - cache[i-2];
    }
    computed_up_to = n;

    return cache[n];
}
