/*
 * A042798  Numerators of continued fraction convergents to sqrt(930).
 * https://oeis.org/A042798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 122*a(n-2) - a(n-4), a(0)=30, a(1)=61, a(2)=3690,
 *            a(3)=7441
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary order of calls */
    static seq_t *cache = NULL;          /* cached sequence values */
    static size_t cap = 0;               /* allocated size of cache */
    static size_t len = 0;               /* number of terms already computed */

    size_t idx = (size_t)n;              /* idx_t is guaranteed to fit in size_t */

    /* grow cache if necessary */
    if (idx + 1 > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (idx + 1) * sizeof(seq_t));
        if (!tmp) exit(1);               /* abort on allocation failure */
        cache = tmp;
        cap = idx + 1;
    }

    /* compute terms up to idx, using recurrence */
    for (size_t i = len; i <= idx; ++i) {
        if (i == 0)          cache[i] = (seq_t)30;
        else if (i == 1)     cache[i] = (seq_t)61;
        else if (i == 2)     cache[i] = (seq_t)3690;
        else if (i == 3)     cache[i] = (seq_t)7441;
        else                 cache[i] = (seq_t)122 * cache[i-2] - cache[i-4];
    }
    len = idx + 1;           /* update cache length */

    return cache[idx];
}
