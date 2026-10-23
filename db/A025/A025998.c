/*
 * A025998  Expansion of 1/((1-2*x)*(1-5*x)*(1-8*x)*(1-10*x)).
 * https://oeis.org/A025998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*a(n-1)-216*a(n-2)+740*a(n-3)-800*a(n-4), with a(0)=1,
 *            a(1)=25, a(2)=409, a(3)=5565
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the nth term of OEIS A025998. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;   /* array of seq_t values            */
    static idx_t cache_size = 0;  /* number of elements currently stored */

    /* ensure the initial values are present */
    if (cache_size == 0) {
        cache_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;  /* out of memory, return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)25;
        cache[2] = (seq_t)409;
        cache[3] = (seq_t)5565;
    }

    /* if the requested n is already cached, just return it */
    if ((idx_t)n < cache_size)
        return cache[n];

    /* grow the cache to accommodate n */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return (seq_t)0;  /* out of memory */
    cache = tmp;

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = cache[i - 1] * (seq_t)25;
        val -= cache[i - 2] * (seq_t)216;
        val += cache[i - 3] * (seq_t)740;
        val -= cache[i - 4] * (seq_t)800;
        cache[i] = val;
    }
    cache_size = new_cap;
    return cache[n];
}
