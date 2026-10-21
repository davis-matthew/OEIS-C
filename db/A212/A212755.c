/*
 * A212755  Number of (w,x,y,z) with all terms in {0,...,n} and
 *          |w-x|=max{w,x,y,z}-min{w,x,y,z}.
 * https://oeis.org/A212755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1)-10*a(n-2)+10*a(n-3)-5*a(n-4)+a(n-5) a(0)=1,
 *            a(1)=10, a(2)=37, a(3)=96, a(4)=205
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of computed terms */
static idx_t cached_size = 0; /* number of entries stored */

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;
        cache[1] = 10;
        cache[2] = 37;
        cache[3] = 96;
        cache[4] = 205;
        cached_size = 5;
    }

    if (n < cached_size)
        return cache[n];

    /* grow cache to contain at least up to n */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute missing terms */
    for (idx_t i = cached_size; i <= n; ++i) {
        cache[i] = 5 * cache[i - 1]
                 - 10 * cache[i - 2]
                 + 10 * cache[i - 3]
                 - 5  * cache[i - 4]
                 +    cache[i - 5];
    }
    cached_size = new_size;
    return cache[n];
}
