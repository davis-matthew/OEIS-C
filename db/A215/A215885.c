/*
 * A215885  a(n) = 3*a(n-1) - a(n-3), with a(0) = 3, a(1) = 3, and a(2) = 9.
 * https://oeis.org/A215885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-3), a(0) = 3, a(1) = 3, a(2) = 9
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t size  = 0;   /* number of terms stored: cache[0..size-1] */

    /* initialise base cases on first call */
    if (size == 0) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        cache[0] = 3;
        cache[1] = 3;
        cache[2] = 9;
        size = 3;
    }

    /* return if already computed */
    if (n < size)
        return cache[n];

    /* grow array to hold up to n, recompute missing terms */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    for (idx_t i = size; i <= n; ++i) {
        cache[i] = (seq_t)3 * cache[i - 1] - cache[i - 3];
    }
    size = n + 1;
    return cache[n];
}
