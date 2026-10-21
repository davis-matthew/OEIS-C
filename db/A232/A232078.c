/*
 * A232078  Number of (2+1) X (n+1) 0..1 arrays with every element equal to
 *          some horizontal, diagonal or antidiagonal neighbor, with top left
 *          element zero.
 * https://oeis.org/A232078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 11*a(n-2) + 23*a(n-3) + 45*a(n-4) + 10*a(n-5)
 *            * + a(n-6) - 3*a(n-7) - 2*a(n-8), offset 1. * Initial values:
 *            a(1)=11, a(2)=87, a(3)=520, a(4)=3681, a(5)=26587, *
 *            a(6)=189404, a(7)=1348429, a(8)=9607995
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms.  1‑indexed array. */
    static seq_t *cache = NULL;
    static size_t   cache_sz = 0;   /* largest computed index (>=8 after init) */
    static size_t   cache_cap= 0;   /* allocated size of cache[] (>=cache_sz+1) */

    if (n < 1) return 0;   /* undefined for n=0, but harmless */

    /* Initialise cache with first 8 terms if not already done. */
    if (cache_sz == 0) {
        const size_t init_count = 8;
        /* Allocate enough space for indices up to 8 (1‑based). */
        cache_cap = init_count * 2;   /* 16 elements gives indices 0‑15 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* memory allocation failure – unlikely */

        /* 0‑th slot unused. */
        static const seq_t init_vals[9] = {
            0,               /* dummy for 0‑th */
            11ULL, 87ULL, 520ULL, 3681ULL,
            26587ULL, 189404ULL, 1348429ULL, 9607995ULL
        };
        for (size_t i = 1; i <= init_count; ++i)
            cache[i] = init_vals[i];

        cache_sz = init_count;
    }

    /* Expand cache if we need to compute terms past the current allocation. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_sz)
        return cache[(size_t)n];

    /* Compute missing terms up to n. */
    for (size_t i = cache_sz + 1; i <= (size_t)n; ++i) {
        /* All indices used in recurrence are guaranteed to be >=1
           because i-8 >= 1 when i >= 9.  For i <= 8 we already have
           the values from the initial array.  The loop starts from
           cache_sz+1, which is at least 9. */
        seq_t val = 5ULL * cache[i-1] + 11ULL * cache[i-2] +
                    23ULL * cache[i-3] + 45ULL * cache[i-4] +
                    10ULL * cache[i-5] + cache[i-6];
        val -= 3ULL * cache[i-7];
        val -= 2ULL * cache[i-8];
        cache[i] = val;
    }
    cache_sz = (size_t)n;

    return cache[(size_t)n];
}
