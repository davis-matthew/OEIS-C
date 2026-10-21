/*
 * A215786  Number of permutations of 0..floor((n*8-1)/2) on even squares of
 *          an nX8 array such that each row, column, diagonal and (downwards)
 *          antidiagonal of even squares is increasing.
 * https://oeis.org/A215786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 272*a(n-2) - 3439*a(n-4) - 3336*a(n-6) + 140*a(n-8) with
 *            initial values: a(1)=1, a(2)=5, a(3)=110, a(4)=932, a(5)=26451,
 *            a(6)=234217, a(7)=6812794, a(8)=60485308 (The sequence starts at
 *            n = 1; values for n < 1 are undefined.)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;     /* array indexed by n (0..max) */
    static idx_t cache_size = 0;    /* allocated size (max index + 1) */
    static idx_t computed = 0;      /* highest index for which cache[n] is valid */

    /* initial values for indices 1..8 */
    static const seq_t init_vals[9] = {
        0,            /* dummy for index 0 */
        1, 5, 110, 932,
        26451, 234217, 6812794, 60485308
    };

    if (n <= 8)                 /* direct lookup for the first 8 terms */
        return init_vals[n];

    /* first invocation: allocate cache and store the first 8 terms */
    if (cache == NULL) {
        cache_size = 9;   /* indices 0..8 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)                         /* allocation failure */
            return (seq_t)0;
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = init_vals[i];
        computed = 8;
    }

    /* extend the cache up to the requested n if needed */
    if (n > computed) {
        idx_t new_size = n + 1;               /* up to index n */
        if (new_size > cache_size) {
            seq_t *tmp = (seq_t *)realloc(cache,
                                           (size_t)new_size * sizeof(seq_t));
            if (!tmp)                         /* allocation failure */
                return (seq_t)0;
            cache = tmp;
            cache_size = new_size;
        }

        /* compute new terms using the recurrence */
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t v1 = (seq_t)272   * cache[(size_t)(i - 2)];
            seq_t v2 = (seq_t)3439 * cache[(size_t)(i - 4)];
            seq_t v3 = (seq_t)3336 * cache[(size_t)(i - 6)];
            seq_t v4 = (seq_t)140   * cache[(size_t)(i - 8)];
            cache[(size_t)i] = v1 - v2 - v3 + v4;
        }
        computed = n;
    }

    return cache[(size_t)n];
}
