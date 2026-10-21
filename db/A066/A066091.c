/*
 * A066091  a(n+2) = a(n+1)*a(n)*(a(n+1)+a(n)), with a(0)=1 and a(1)=2.
 * https://oeis.org/A066091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(0)=1, a(1)=2, a(n+2)=a(n+1)*a(n)*(a(n+1)+a(n))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute a(n) */
seq_t a(idx_t n)
{
    /* static cache for memoized values */
    static seq_t *cache = NULL;
    static idx_t size = 0;          /* number of stored terms */

    /* initialise the cache with the first two terms */
    if (cache == NULL) {
        size = 2;
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
    }

    /* if we already have the term, return it */
    if (n < size)
        return cache[n];

    /* grow the cache up to n+1 elements */
    if (size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort the program (cannot recover) */
            abort();
        }
        cache = tmp;

        /* compute missing terms sequentially */
        for (idx_t i = size; i <= n; ++i) {
            seq_t an1 = cache[i - 1];
            seq_t an2 = cache[i - 2];
            cache[i] = an1 * an2 * (an1 + an2);
        }
        size = new_size;
    }

    return cache[n];
}
