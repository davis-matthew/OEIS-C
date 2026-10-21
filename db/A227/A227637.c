/*
 * A227637  Number of n X 2 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 3 binary array having determinant equal to one,
 *          with rows and columns of the latter in nondecreasing lexicographic
 *          order.
 * https://oeis.org/A227637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) for
 *            n>6. Initial values: a(1)=2, a(2)=5, a(3)=11, a(4)=23, a(5)=44,
 *            a(6)=78
 * Range:     u8 n <= 8, u16 n <= 35, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;
static idx_t cache_max = 0;     /* largest n for which cache[n] is defined */
static size_t cache_cap = 0;    /* allocated length */

/* Expand cache to at least n+1 elements and compute any missing values. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max)
        return;

    /* Resize array if needed. */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = alloc_seq_array(cache_cap);
        /* Zero all elements to be safe. */
        for (idx_t i = 0; i < cache_cap; ++i)
            cache[i] = 0;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = alloc_seq_array(new_cap);
        for (size_t i = 0; i < cache_cap; ++i)
            tmp[i] = cache[i];
        free_seq_array(cache);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values from current max+1 up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1)      cache[i] = 2;
        else if (i == 2) cache[i] = 5;
        else if (i == 3) cache[i] = 11;
        else if (i == 4) cache[i] = 23;
        else if (i == 5) cache[i] = 44;
        else if (i == 6) cache[i] = 78;
        else {
            seq_t v = 5 * cache[i - 1] + 10 * cache[i - 3] + cache[i - 5];
            v -= 10 * cache[i - 2];
            v -= 5  * cache[i - 4];
            cache[i] = v;
        }
    }

    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n<1, return 0 as safe default */
    ensure_cache(n);
    return cache[n];
}
