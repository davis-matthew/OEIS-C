/*
 * A216263  Expansion of 1 / ((1-2*x)*(1-4*x+x^2)).
 * https://oeis.org/A216263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + 2*a(n-3), a(0) = 1, a(1) = 6, a(2)
 *            = 27
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of OEIS sequence A216263. */
seq_t a(idx_t n)
{
    /* Static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;       /* current allocated size */
    static size_t max_computed = 0;    /* highest index computed so far */

    /* Ensure we have room for index n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_len) {
        size_t new_len = cache_len ? cache_len << 1 : 4;
        while (new_len < needed)
            new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;
        cache_len = new_len;
    }

    /* Handle the base cases directly */
    if (n == 0) {
        cache[0] = 1;
        max_computed = 0;
        return 1;
    }
    if (n == 1) {
        cache[1] = 6;
        max_computed = 1;
        return 6;
    }
    if (n == 2) {
        cache[2] = 27;
        max_computed = 2;
        return 27;
    }

    /* Ensure base values are stored (they may be missing after realloc) */
    if (cache[0] == 0) cache[0] = 1;
    if (cache[1] == 0) cache[1] = 6;
    if (cache[2] == 0) cache[2] = 27;

    /* Compute any missing terms up to n */
    if ((size_t)n > max_computed) {
        size_t start = max_computed + 1;
        if (start < 3) start = 3;          /* start from first computed term */
        for (size_t i = start; i <= (size_t)n; ++i) {
            /* Recurrence: a(i) = 6*a(i-1) - 9*a(i-2) + 2*a(i-3) */
            cache[i] = 6 * cache[i - 1] - 9 * cache[i - 2] + 2 * cache[i - 3];
        }
        max_computed = (size_t)n;
    }

    return cache[n];
}
