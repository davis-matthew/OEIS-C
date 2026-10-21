/*
 * A212577  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          |w-x|=2|x-y|-|y-z|.
 * https://oeis.org/A212577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            2*a(n-1)-a(n-2)+2*a(n-3)-4*a(n-4)+2*a(n-5)-a(n-6)+2*a(n-7)-a(n-8)
 *            with initial values a(0)=0, a(1)=1, a(2)=4, a(3)=17, a(4)=46,
 *            a(5)=89, a(6)=154, a(7)=251
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

static seq_t *cache = NULL;
static idx_t cache_len = 0;              /* number of terms stored */
static size_t cache_cap = 0;             /* allocated capacity */

/* Allocate or grow the cache to at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        exit(1);   /* allocation failed */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Predefined initial values for n = 0..7. */
static const seq_t init_vals[8] = {
    (seq_t)0, (seq_t)1, (seq_t)4, (seq_t)17,
    (seq_t)46, (seq_t)89, (seq_t)154, (seq_t)251
};

seq_t a(idx_t n)
{
    /* Fast path for the first eight terms – no caching needed. */
    if (n < 8u)
        return init_vals[n];

    /* First call with n >= 8: create the cache and store the seeds. */
    if (!cache) {
        grow_cache(8);
        for (idx_t i = 0; i < 8u; ++i)
            cache[i] = init_vals[i];
        cache_len = 8u;
    }

    /* Compute all terms up to n, if not already computed. */
    if (cache_len <= n) {
        grow_cache((size_t)(n + 1));
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t t =
                (cache[i - 1] << 1) - cache[i - 2]
                + (cache[i - 3] << 1) - (cache[i - 4] * (seq_t)4)
                + (cache[i - 5] << 1) - cache[i - 6]
                + (cache[i - 7] << 1) - cache[i - 8];
            cache[i] = t;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
