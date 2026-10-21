/*
 * A240193  Number of 3 X n 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or one plus the sum of the elements
 *          above it or three plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 27*a(n-2) + 29*a(n-3) + 6*a(n-4) - 32*a(n-5) +
 *            16*a(n-6) for n > 9 Initial values: a(1)=3, a(2)=12, a(3)=37,
 *            a(4)=129, a(5)=478, a(6)=1908, a(7)=7868, a(8)=32888,
 *            a(9)=138292
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation: cache[0] unused, cache[1] = a(1), … */
    static seq_t *cache = NULL;   /* array of terms, cache[0] unused */
    static idx_t cache_len = 0;   /* highest index stored (0 if none) */
    static size_t   cache_cap = 0;/* allocated capacity (elements including unused 0) */

    /* Ensure the cache can hold index n */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first 9 terms on first use or when cache is too small */
    if (cache_len == 0 || cache_len < 9) {
        static const seq_t init[10] = {
            (seq_t)0,      /* index 0 unused */
            (seq_t)3,      /* a(1) */
            (seq_t)12,     /* a(2) */
            (seq_t)37,     /* a(3) */
            (seq_t)129,    /* a(4) */
            (seq_t)478,    /* a(5) */
            (seq_t)1908,   /* a(6) */
            (seq_t)7868,   /* a(7) */
            (seq_t)32888,  /* a(8) */
            (seq_t)138292  /* a(9) */
        };
        idx_t m = (cache_len < 9) ? 9 : cache_len;
        if (m > n) m = n;        /* avoid writing beyond needed */
        for (idx_t i = 1; i <= m; ++i) {
            cache[i] = init[i];
        }
        cache_len = m;
    }

    /* Return early if requested index is already available */
    if (n <= cache_len) return cache[n];

    /* Compute terms up to n using the recurrence (valid for n>9) */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* All indices i-1 … i-6 are valid because i > 9 from this point */
        seq_t term = cache[i-1] * (seq_t)9
                    - cache[i-2] * (seq_t)27
                    + cache[i-3] * (seq_t)29
                    + cache[i-4] * (seq_t)6
                    - cache[i-5] * (seq_t)32
                    + cache[i-6] * (seq_t)16;
        cache[i] = term;
    }
    cache_len = n;
    return cache[n];
}
