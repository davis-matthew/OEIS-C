/*
 * A229695  Number of defective 3-colorings of a 2Xn 0..2 array connected
 *          horizontally and antidiagonally with exactly two mistakes, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 27*a(n-2) + 27*a(n-3) for n>6 Initial values:
 *            a(1)=0, a(2)=3, a(3)=43, a(4)=245, a(5)=1171, a(6)=5077
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;          /* capacity of cached array */
static idx_t cache_max = 0;           /* largest index that has been computed */

/* enlarge the cache to hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= (size_t)(n + 1))
        new_cap *= 2;

    seq_t *newp;
#ifdef __cplusplus
    /* In C++ allocate an array with proper constructors */
    newp = new seq_t[new_cap];
    if (cache) {
        /* copy existing values */
        for (size_t i = 0; i < cache_cap; ++i)
            newp[i] = cache[i];
        delete[] cache;
    }
#else
    /* In C use realloc */
    newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (!newp)
        abort();   /* allocation failure */

    if (!cache) {
#ifdef __cplusplus
        /* constructors already called for new array, nothing more to do */
#else
        /* zero-initialize the new area in C */
        memset(newp, 0, new_cap * sizeof(seq_t));
#endif
    } else {
#ifndef __cplusplus
        /* in C, only the new region needs zeroing */
        size_t old_cap = cache_cap;
        memset(newp + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));
#endif
    }

    cache = newp;
    cache_cap = new_cap;
}

/* compute values up to n */
static void compute_upto(idx_t n)
{
    if (cache_max >= n)
        return;
    ensure_capacity(n);

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 0;
        } else if (i == 2) {
            val = 3;
        } else if (i == 3) {
            val = 43;
        } else if (i == 4) {
            val = 245;
        } else if (i == 5) {
            val = 1171;
        } else if (i == 6) {
            val = 5077;
        } else {
            val = 9 * cache[i - 1] - 27 * cache[i - 2] + 27 * cache[i - 3];
        }
        cache[i] = val;
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined below offset, return 0 */
    compute_upto(n);
    return cache[n];
}
