/*
 * A204734  Number of (n+1)X3 0..1 arrays with rows and columns of permanents
 *          of all 2X2 subblocks lexicographically nondecreasing, and all 2X2
 *          permanents nonzero.
 * https://oeis.org/A204734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 8*a(n-3) + 11*a(n-4) - 4*a(n-5)
 *            with initial values a(1)=15, a(2)=43, a(3)=115, a(4)=304,
 *            a(5)=787
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values, 1‑based */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* highest n that has been computed */
    static size_t cache_cap = 0;   /* allocated capacity in elements */

    /* initialise cache with the first five terms */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failure – unlikely */
        cache[0] = 0;              /* unused sentinel for 1‑based indexing */
        cache[1] = 15;
        cache[2] = 43;
        cache[3] = 115;
        cache[4] = 304;
        cache[5] = 787;
        cache_size = 5;
    }

    if (n <= cache_size)
        return cache[n];

    /* grow the storage if needed */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute remaining terms up to n using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t pos = 4 * cache[i-1];
        pos += 11 * cache[i-4];
        seq_t neg = 2 * cache[i-2];
        neg += 8 * cache[i-3];
        neg += 4 * cache[i-5];
        cache[i] = pos - neg;    /* final subtraction is safe because pos >= neg */
    }
    cache_size = n;

    return cache[n];
}
