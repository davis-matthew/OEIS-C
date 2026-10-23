/*
 * A187498  Let i be in {1,2,3,4} and let r >= 0 be an integer. Let p={p_1,
 *          p_2, p_3, p_4} = {-3,0,1,2}, n=3*r+p_i, and define a(-3)=0. Then
 *          a(n)=a(3*r+p_i) gives the quantity of H_(9,4,0) tiles in a
 *          subdivided H_(9,i,r) tile after linear scaling by the factor Q^r,
 *          where Q=sqrt(2*cos(Pi/9)).
 * https://oeis.org/A187498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + 3*a(n-6) - 2*a(n-9) - a(n-12) for n ≥ 12;
 *            initial values a(0)=0, a(1)=0, a(2)=1, a(3)=0, a(4)=1, a(5)=1,
 *            a(6)=1, a(7)=1, a(8)=2, a(9)=1, a(10)=3, a(11)=3
 * Range:     u8 n <= 32, u16 n <= 58, u32 n <= 111, u64 n <= 216,
 *            u128 n <= 427, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static const seq_t init_vals[12] = {
    0, 0, 1, 0, 1, 1, 1, 1, 2, 1, 3, 3
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated length of cache */
static idx_t  cache_max = 0;   /* highest index computed so far */

/* Ensure that the cache can hold indices up to "needed - 1" */
static void ensure_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = needed;
#if defined(__cplusplus)
    seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
    seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (!tmp)
        exit(1);         /* out of memory – abort silently */

    cache = tmp;
    cache_cap = new_cap;

    /* On the first allocation, initialise the first 12 terms */
    if (cache_max == 0 && cache_cap >= 12) {
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init_vals[i];
        cache_max = 11;   /* we've computed a(0)..a(11) */
    }
}

seq_t a(idx_t n)
{
    if (n < 12)
        return init_vals[(size_t)n];

    ensure_cache((size_t)n + 1);

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t t = cache[(size_t)(i-3)] + (seq_t)3 * cache[(size_t)(i-6)];
        t -= (seq_t)2 * cache[(size_t)(i-9)];
        t -= cache[(size_t)(i-12)];
        cache[(size_t)i] = t;
    }

    cache_max = n;
    return cache[(size_t)n];
}
