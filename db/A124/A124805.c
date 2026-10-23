/*
 * A124805  Number of circular n-letter words over the alphabet {0,1,2,3} with
 *          adjacent letters differing by at most 2.
 * https://oeis.org/A124805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - 2*a(n-3) (n > 3) Initial terms:
 *            a(0)=1, a(1)=4, a(2)=14, a(3)=46
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* if cache is empty, allocate and set base values */
    if (cache == NULL) {
        cache = (seq_t*)malloc(4 * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure, return 0 */
        cache[0] = 1;
        cache[1] = 4;
        cache[2] = 14;
        cache[3] = 46;
        cache_size = 4;
    }

    /* grow cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure */
        cache = tmp;
        /* compute new values using recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) cache[i] = 1;
            else if (i == 1) cache[i] = 4;
            else if (i == 2) cache[i] = 14;
            else if (i == 3) cache[i] = 46;
            else
                cache[i] = (seq_t)4 * cache[i-1]
                           - cache[i-2]
                           - (seq_t)2 * cache[i-3];
        }
        cache_size = new_size;
    }
    return cache[n];
}
