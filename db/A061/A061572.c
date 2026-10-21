/*
 * A061572  a(n) = (n!)^2 * Sum_{k=1..n} 1/(k^2*(k-1)!).
 * https://oeis.org/A061572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 + n - 1)*a(n-1) - (n-1)^3*a(n-2), a(1)=1, a(2)=5
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes a(n) for OEIS sequence A061572 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0)
        return 0;   /* offset starts at 1, value for n=0 unused */

    /* Lazy initialization of the cache with the first two terms */
    if (!cache) {
        cache_size = 3;                     /* indices 0..2 */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = 0;   /* unused placeholder for index 0 */
        cache[1] = 1;   /* a(1) */
        cache[2] = 5;   /* a(2) */
    }

    /* Resize cache and compute any missing values up to n */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        if (new_size < 4)
            new_size = 4;                          /* ensure room for loop bounds */
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;

        /* Compute recurrence for all indices >=3 up to n */
        idx_t start = cache_size;
        if (start < 3)
            start = 3;
        for (idx_t i = start; i <= n; ++i) {
            seq_t im1 = (seq_t)(i - 1);                /* (i-1) as seq_t */
            seq_t term1 = (seq_t)i * ((seq_t)i + 1);    /* i^2 + i */
            term1 = term1 - 1;                         /* i^2 + i - 1 */
            seq_t part1 = term1 * cache[i - 1];
            seq_t part2 = im1 * im1 * im1 * cache[i - 2];
            cache[i] = part1 - part2;
        }
        cache_size = new_size;
    }

    return cache[n];
}
