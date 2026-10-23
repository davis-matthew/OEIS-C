/*
 * A019485  a(n) = 2*a(n-1) + 2*a(n-2) - 3*a(n-3), with a(0) = 2, a(1) = 5,
 *          a(2) = 12.
 * https://oeis.org/A019485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 3*a(n-3) Initial values: a(0) = 2,
 *            a(1) = 5, a(2) = 12
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        /* Grow cache to include index n */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failed, return zero */
        cache   = tmp;
        /* Compute new terms starting from old cache_size */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0)        cache[0] = (seq_t)2;
            else if (i == 1)   cache[1] = (seq_t)5;
            else if (i == 2)   cache[2] = (seq_t)12;
            else {
                /* a(i) = 2*a(i-1) + 2*a(i-2) - 3*a(i-3) */
                seq_t term = cache[i-1] + cache[i-1];          /* 2*a(i-1) */
                term = term + cache[i-2] + cache[i-2];         /* +2*a(i-2) */
                term = term - cache[i-3] - cache[i-3] - cache[i-3]; /* -3*a(i-3) */
                cache[i] = term;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
