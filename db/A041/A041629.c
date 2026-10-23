/*
 * A041629  Denominators of continued fraction convergents to sqrt(333).
 * https://oeis.org/A041629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A041629: Denominators of continued fraction convergents to sqrt(333)
 * Recurrence: a(n) = 146*a(n-2) - a(n-4)   (n > 3)
 * Initial values: a(0) = 1, a(1) = 4, a(2) = 145, a(3) = 584
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of computed terms          */
    static idx_t  cache_len = 0;     /* number of computed terms         */

    /* Ensure the cache is large enough for indices up to n.        */
    if (n + 1 > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) {           /* allocation failed – return 0   */
            free(cache);
            cache = NULL;
            cache_len = 0;
            return (seq_t)0;
        }
        cache = tmp;
    }

    /* Compute missing terms, starting from the first unknown index. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)            cache[i] = (seq_t)1;
        else if (i == 1)       cache[i] = (seq_t)4;
        else if (i == 2)       cache[i] = (seq_t)145;
        else if (i == 3)       cache[i] = (seq_t)584;
        else                   cache[i] = 146 * cache[i - 2] - cache[i - 4];
    }

    /* Update how many terms are now stored.                          */
    if (n + 1 > cache_len) cache_len = n + 1;

    return cache[n];
}
