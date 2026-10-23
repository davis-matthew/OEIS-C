/*
 * A131298  Diagonal sums of A104730.
 * https://oeis.org/A131298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n‑1) + 2*a(n‑2) – 2*a(n‑4) – a(n‑5); with initial
 *            values a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=6, a(5)=11. This is
 *            the diagonal‑sum form of A104730
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of elements stored (always ≥ n+1 after construction) */

    /* initialise the cache on the first call */
    if (cache_size == 0) {
        cache_size = 6; /* a(0) … a(5) are known */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failed – return 0 as error value */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)6;
        cache[5] = (seq_t)11;
    }

    /* if the requested index is already cached, return it immediately */
    if (n < cache_size)
        return cache[n];

    /* grow the cache if necessary */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)          /* reallocation failed – return 0 as error value */
        return (seq_t)0;
    cache = tmp;

    /* compute any missing terms up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = cache[i-1] + (seq_t)2 * cache[i-2]
                        - (seq_t)2 * cache[i-4] - cache[i-5];
        cache[i] = val;
    }
    cache_size = new_size;
    return cache[n];
}
