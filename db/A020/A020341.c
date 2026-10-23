/*
 * A020341  Expansion of 1/((1-5x)(1-7x)(1-12x)).
 * https://oeis.org/A020341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) - 179*a(n-2) + 420*a(n-3) with a(0)=1, a(1)=24,
 *            a(2)=397
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static idx_t last_computed = 2;

    /* Direct outputs for the base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)397;

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_len = 3;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;               /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)24;
        cache[2] = (seq_t)397;
        last_computed = 2;
    }

    /* Grow cache if needed to hold up to index n */
    size_t required = (size_t)(n + 1);
    if (required > cache_len) {
        seq_t *tmp = (seq_t*)realloc(cache, required * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* allocation failed */
        cache = tmp;
        cache_len = required;
    }

    /* Compute missing terms up to n using the recurrence */
    while (last_computed < n) {
        idx_t i = last_computed + 1;
        cache[i] = (seq_t)24 * cache[i-1]
                 - (seq_t)179 * cache[i-2]
                 + (seq_t)420 * cache[i-3];
        last_computed = i;
    }

    return cache[n];
}
