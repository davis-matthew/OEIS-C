/*
 * A139217  Smallest positive integer of the form 3k+1 such that all subsets
 *          of {a(1),...,a(n)} have a different sum.
 * https://oeis.org/A139217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3) (for n >= 6) Base values (n =
 *            1..5): a(1) = 1 a(2) = 4 a(3) = 7 a(4) = 13 a(5) = 22 The
 *            function caches computed terms; it is safe for calls in any
 *            order and runs in O(n) time with modest memory
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms (0‑based) */
static size_t cache_cap = 0;         /* allocated capacity */
static size_t cache_sz  = 0;         /* number of terms stored (max index+1) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined, but harmless */

    /* Return pre‑computed base terms directly for n ≤ 5 */
    if (n <= 5) {
        static const seq_t base[5] = {1, 4, 7, 13, 22};
        return base[(size_t)n - 1];
    }

    /* Initialise cache with first five values if not present */
    if (cache == NULL) {
        cache_cap = 8;                /* initial capacity */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = 1;                /* a(1) */
        cache[1] = 4;                /* a(2) */
        cache[2] = 7;                /* a(3) */
        cache[3] = 13;               /* a(4) */
        cache[4] = 22;               /* a(5) */
        cache_sz = 5;
    }

    /* Grow the cache array if needed */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap < 8 ? 8 : cache_cap;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    while (cache_sz < (size_t)n) {
        size_t i = cache_sz;          /* index of new term (0‑based) */
        seq_t t1 = cache[i-1];        /* a(i)   */
        seq_t t2 = cache[i-2];        /* a(i-1) */
        seq_t t3 = cache[i-3];        /* a(i-2) */
        cache[i] = t1 + t2 + t3 + t3; /* a(i+1) = a(i) + a(i-1) + 2*a(i-2) */
        ++cache_sz;
    }

    return cache[(size_t)n - 1];
}
