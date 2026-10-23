/*
 * A182435  a(n) = 6*a(n-1) - a(n-2) - 2 with n>1, a(0)=0, a(1)=1.
 * https://oeis.org/A182435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1; for n>1: a(n) = 6*a(n-1) - a(n-2) - 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values of the sequence */
    static idx_t cache_size = 0;  /* number of entries stored in cache */

    /* ensure the first two terms are available */
    if (cache_size == 0) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* answer directly for the first two terms */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* grow the cache if needed and compute new terms */
    if (cache_size <= n) {
        idx_t i;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;              /* reallocate may move the block */
        for (i = cache_size; i <= n; ++i) {
            seq_t val = cache[i-1] * (seq_t)6 - cache[i-2] - (seq_t)2;
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n];
}
