/*
 * A229583  Number of defective 3-colorings of an n X 5 0..2 array connected
 *          horizontally and antidiagonally with exactly one mistake, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1) - 261*a(n-2) + 540*a(n-3) - 324*a(n-4) Initial
 *            values: a(1)=16, a(2)=444, a(3)=9156, a(4)=167364
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t cached = 0;

    /* Ensure base cases are available */
    if (cached < 4) {
        if (cap < 4) {
            cache = (seq_t *)realloc(cache, 4 * sizeof(seq_t));
            cap = 4;
        }
        cache[0] = (seq_t)16;
        cache[1] = (seq_t)444;
        cache[2] = (seq_t)9156;
        cache[3] = (seq_t)167364;
        cached = 4;
    }

    if (n <= cached) {
        return cache[n - 1];
    }

    /* Grow storage if necessary */
    if (n > cap) {
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        cap = n;
    }

    /* Compute terms from cached+1 up to n */
    for (idx_t i = cached + 1; i <= n; ++i) {
        idx_t idx1 = i - 2; /* a(i-1) */
        idx_t idx2 = i - 3; /* a(i-2) */
        idx_t idx3 = i - 4; /* a(i-3) */
        idx_t idx4 = i - 5; /* a(i-4) */

        seq_t term = 0;
        term = 30 * cache[idx1];
        term -= 261 * cache[idx2];
        term += 540 * cache[idx3];
        term -= 324 * cache[idx4];
        cache[i - 1] = term;
    }

    cached = n;
    return cache[n - 1];
}
