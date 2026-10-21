/*
 * A057086  Scaled Chebyshev U-polynomials evaluated at sqrt(10)/2.
 * https://oeis.org/A057086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10 * (a(n-1) - a(n-2)), with a(-1)=0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* Initialize the cache with the first two terms */
    if (len == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)10;         /* a(1) derived from a(0) and a(-1)=0 */
        len = 2;
    }

    /* Grow the cache up to the requested index */
    while (len <= n) {
        if (len == cap) {
            size_t new_cap = cap * 2;
            if (new_cap == 0) new_cap = 16;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cap = new_cap;
        }
        seq_t val = (cache[len-1] - cache[len-2]) * (seq_t)10;
        cache[len] = val;
        ++len;
    }

    return cache[n];
}
