/*
 * A008033  Coordination sequence T2 for Zeolite Code APC.
 * https://oeis.org/A008033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-5) - a(n-6) + a(n-9) - a(n-10) - a(n-14) +
 *            a(n-15) Initial terms: a(0)=1, a(1)=4, a(2)=10, a(3)=20,
 *            a(4)=35, a(5)=54, a(6)=76, a(7)=104, a(8)=136, a(9)=171,
 *            a(10)=211, a(11)=254, a(12)=302, a(13)=355, a(14)=411, a(15)=471
 * Range:     u8 n <= 11, u16 n <= 177, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static const seq_t init[16] = {1,4,10,20,35,54,76,104,136,171,211,254,302,355,411,471};

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;

    /* Fast path for small n */
    if (n < 16)
        return init[(size_t)n];

    /* Initialise cache on first invocation */
    if (cache == NULL) {
        size_t init_size = 16;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        for (size_t i = 0; i < init_size; ++i)
            cache[i] = init[i];
        cache_max = 15;
    }

    size_t nn = (size_t)n;
    if (nn <= (size_t)cache_max)
        return cache[nn];

    /* Resize cache to accommodate up to n */
    size_t new_size = nn + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL)
        return init[15];   /* unlikely: fall back to a known value */

    cache = tmp;

    /* Compute missing terms up to n */
    for (size_t i = (size_t)cache_max + 1; i <= nn; ++i) {
        cache[i] = cache[i-1] + cache[i-5] - cache[i-6] + cache[i-9] - cache[i-10] - cache[i-14] + cache[i-15];
    }

    cache_max = nn;
    return cache[nn];
}
