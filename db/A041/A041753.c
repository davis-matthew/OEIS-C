/*
 * A041753  Denominators of continued fraction convergents to sqrt(396).
 * https://oeis.org/A041753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 398*a(n-4) - a(n-8) for n>7
   with a(0)=1, a(1)=1, a(2)=9, a(3)=10,
   a(4)=389, a(5)=399, a(6)=3581, a(7)=3980
*/
#include <stdlib.h>
#include <stddef.h>

static const seq_t init[8] = {1, 1, 9, 10, 389, 399, 3581, 3980};

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n < 8)
        return init[n];

    /* Static cache for memoised values */
    static seq_t *cache = NULL;   /* array of computed a(i) */
    static idx_t last_computed = 7;  /* highest index computed so far */

    /* Allocate cache on first use */
    if (cache == NULL) {
        size_t sz = (size_t)n + 1;
        if (sz < 8)
            sz = 8;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache)
            abort();
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init[i];
    }

    /* Extend cache if needed */
    if (n > last_computed) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;

        /* Compute new values from last_computed+1 up to n */
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            cache[i] = (seq_t)398 * cache[i - 4] - cache[i - 8];
        }
        last_computed = n;
    }

    return cache[n];
}
