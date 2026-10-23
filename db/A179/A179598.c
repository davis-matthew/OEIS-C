/*
 * A179598  Eight white kings and one red king on a 3 X 3 chessboard. G.f.: (1
 *          + 2*x)/(1 - 3*x - 8*x^2).
 * https://oeis.org/A179598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 8*a(n-2), with a(0) = 1, a(1) = 5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialize cache with first two terms */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
    }

    /* if already computed, return */
    if (n < cache_len) {
        return cache[n];
    }

    /* extend cache up to n */
    idx_t target = n + 1;                 /* required size */
    if (target > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, target * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* compute new terms using recurrence */
        for (idx_t i = cache_len; i < target; ++i) {
            cache[i] = ((seq_t)3 * cache[i - 1]) + ((seq_t)8 * cache[i - 2]);
        }
        cache_len = target;
    }

    return cache[n];
}
