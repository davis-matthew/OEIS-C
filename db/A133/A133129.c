/*
 * A133129  Number of black/white colorings of a 3 X n rectangle which have no
 *          monochromatic 2 by 2 subsquares.
 * https://oeis.org/A133129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 3*a(n-2) – 2*a(n-3) (n ≥ 3) a(0)=1, a(1)=8,
 *            a(2)=50
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;

    /* Ensure base cases are available */
    if (cache == NULL) {
        cache = (seq_t *)calloc(3, sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* out of memory, return 0 as fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)50;
        cache_sz = 3;
    }

    /* If the requested index is already cached, just return it */
    if ((size_t)n < cache_sz)
        return cache[(size_t)n];

    /* Need to grow cache up to index n */
    size_t target = (size_t)n;
    if (target + 1 > cache_sz) {
        size_t new_sz = target + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;    /* out of memory */
        cache = tmp;

        /* Compute from the first missing index up to target */
        size_t start = cache_sz;           /* first index to compute */
        if (start < 3) start = 3;           /* base cases already filled */
        for (size_t i = start; i <= target; ++i) {
            cache[i] = 6 * cache[i - 1] + 3 * cache[i - 2] - 2 * cache[i - 3];
        }
        cache_sz = new_sz;
    }

    return cache[(size_t)n];
}
